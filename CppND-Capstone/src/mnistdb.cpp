#include "mnistdb.h"

#include "opencv2/ml.hpp"
#include "opencv2/core.hpp"

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <memory>
#include <cstdint>

using namespace std;
using namespace cv;
using namespace cv::ml;


Mnistdb::Mnistdb() { }


Mnistdb::~Mnistdb() {}


int Mnistdb::ReadTrainImg(const string &fn)
{
	ifstream file(fn, ifstream::binary);
	if (!file) {
		cout << "ERROR: in func ReadTrainImg(), fn is: " << fn << endl;
		return 0;
	}

	int dummy;
	file.read((char*)&dummy, sizeof(dummy));
	cout << endl << "In func ReadTrainImg, before swapping, magic_number is: " << dummy << endl;
    dummy = __builtin_bswap32(dummy); 
	cout << "In func ReadTrainImg, after swapping, magic_number is: " << dummy << endl;

	file.read((char*)&dummy, sizeof(dummy));
    m_NumTrainSample = __builtin_bswap32(dummy); 
	file.read((char*)&dummy, sizeof(dummy));
    m_NumImgRow = __builtin_bswap32(dummy);
	file.read((char*)&dummy, sizeof(dummy));
    m_NumImgCol = __builtin_bswap32(dummy);
	cout << "m_NumTrainSample = " << m_NumTrainSample << ", m_NumImgRow = " << m_NumImgRow
		<< ", m_NumImgCol = " << m_NumImgCol << endl;

	char* all_img = new char[m_NumTrainSample*m_NumImgRow*m_NumImgCol];
	file.read(all_img, m_NumTrainSample*m_NumImgRow*m_NumImgCol);
	// m_TrainImg = Mat(m_NumTrainSample, m_NumImgRow*m_NumImgCol, CV_8UC1, all_img); // worked
    m_TrainImg = Mat(m_NumTrainSample, m_NumImgRow*m_NumImgCol, CV_8UC1, all_img).clone();
    delete[] all_img;

	cout << "Successfully read all the training images!\n\n";

	return 0;
}


int Mnistdb::ReadTrainLbl(const string &fn)
{
	ifstream file(fn, ifstream::binary);
	if (!file) {
		cout << "ERROR in func ReadTrainLbl, fn is: " << fn << endl;
		return 0;
	}

	int dummy;
	file.read((char*)&dummy, sizeof(dummy));
    dummy = __builtin_bswap32(dummy);
	file.read((char*)&dummy, sizeof(dummy));
    m_NumTrainSample = __builtin_bswap32(dummy);
	cout << "In func ReadTrainLbl, m_NumTrainSample = " << m_NumTrainSample << endl << endl;

	uint8_t lbl;
	for (int i = 0; i < m_NumTrainSample; i++) {
		file.read((char*)&lbl, sizeof(lbl));
		m_TrainLbl.push_back(lbl);
	}

	return 0; 
}


int Mnistdb::ReadTestImg(const string &fn)
{
	ifstream file(fn, ifstream::binary);
	if (!file) {
		cout << "ERROR in func ReadTestImg, fn is: " << fn << endl;
		return 0;
	}

	int dummy;
	file.read((char*)&dummy, sizeof(dummy));
	file.read((char*)&dummy, sizeof(dummy));
    m_NumTestSample = __builtin_bswap32(dummy);
	file.read((char*)&dummy, sizeof(dummy));
    m_NumImgRow = __builtin_bswap32(dummy);
	file.read((char*)&dummy, sizeof(dummy));
    m_NumImgCol = __builtin_bswap32(dummy);
	cout << "In func ReadTestImg, m_NumTestSample = " << m_NumTestSample << ", m_NumImgRow = " << m_NumImgRow
		<< ", m_NumImgCol = " << m_NumImgCol << endl << endl;

	char* all_img = new char[m_NumTestSample*m_NumImgRow*m_NumImgCol];
	file.read(all_img, m_NumTestSample*m_NumImgRow*m_NumImgCol);
	// m_TestImg = Mat(m_NumTestSample, m_NumImgRow*m_NumImgCol, CV_8UC1, all_img);
    m_TestImg = Mat(m_NumTestSample, m_NumImgRow*m_NumImgCol, CV_8UC1, all_img).clone();
    delete[] all_img;

	return 0;
}


int Mnistdb::ReadTestLbl(const string &fn)
{
	ifstream file(fn, ifstream::binary | ifstream::in);
	if (!file) {
		cout << "ERROR in func ReadTestLbl, fn is: " << fn << endl; 
		return 0;
	}

	int dummy;
	file.read((char*)&dummy, 4);
	file.read((char*)&dummy, sizeof(dummy));
    m_NumTestSample = __builtin_bswap32(dummy);
	cout << "In func ReadTestLbl, m_NumTestSample = " << m_NumTestSample << endl << endl;

	uint8_t lbl;
	for (int i = 0; i < m_NumTestSample; i++) {
		file.read((char*)&lbl, 1);
		m_TestLbl.push_back(lbl);
	}

	return 0; 
}


// works, accuracy: 89%
void Mnistdb::SVM_Classifier()
{
	Ptr<SVM> svm; 
	svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 500, 1e-6));

	m_TrainImg.convertTo(m_TrainImg, CV_32FC1);
	
	Mat train_lbl = Mat::zeros(m_NumTrainSample, 1, CV_8UC1);
	for (int i = 0; i < m_NumTrainSample; i++)
			train_lbl.at<uchar>(i, 0) = m_TrainLbl[i];
	train_lbl.convertTo(train_lbl, CV_32SC1);

	cout << endl << "SVM, start training ... " << endl; 
	svm->train(m_TrainImg, ROW_SAMPLE, train_lbl);
	cout << "SVM training finished, saving training model to SVM_MNIST.xml" << endl;
	svm->save("SVM_MNIST.xml");

	m_TestImg.convertTo(m_TestImg, CV_32FC1);
	cout << endl << endl << "Predicting ... " << endl;
	int correct_count = 0;
	for (int i = 0; i < m_TestImg.rows; i++) {
		float response = svm->predict(m_TestImg.row(i));
		if (m_TestLbl[i] = (uchar)response)
			correct_count++;
        
        // output a few results
        if (i%1000 == 0)
		    cout << "Test sample # = " << i << ", actual digit = " << (int)m_TestLbl[i] << ", predicted digit = " << (int)response << endl;
	}

	float correct_ratio = 1.0*correct_count/m_NumTestSample; 
    cout << endl << correct_count << " hand-written digits out of " << m_NumTestSample << " are recognized correctly!" << endl;
	cout << "correct_ratio = " << correct_ratio << endl; 

	return; 
}