#ifndef MNISTDB_H
#define MNISTDB_H

#include "opencv2/opencv.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <cstdint>

using namespace cv;
using namespace std;

class Mnistdb
{
public:
	// constructor
	Mnistdb();

	// destructor
	~Mnistdb();

	// getter
	int NumImgRow() const noexcept { return m_NumImgRow; }
	int NumImgCol() const noexcept { return m_NumImgCol; }

	Mat &TrainImg()  { return m_TrainImg; }  //if Mat, then not to have "const noexcept"
	auto &TrainLbl() const noexcept { return m_TrainLbl; }
	int  NumTrainSample() const noexcept { return m_NumTrainSample; }

	auto &TestImg() const noexcept { return m_TestImg; }
	auto &TestLbl() const noexcept { return m_TestLbl; }
	int NumTestSample() const noexcept { return m_NumTestSample; }

	// setter
	void SetNumImgRow(const int num_img_row) { m_NumImgRow = num_img_row; }
	void SetNumImgCol(const int num_img_col) { m_NumImgCol = num_img_col; }

	// member functions
	int ReadTrainImg(const string &fn);
	int ReadTrainLbl(const string &fn);

	int ReadTestImg (const string &fn);
	int ReadTestLbl (const string &fn);

	// ML classifiers
	// void LR_Classifier();
	void SVM_Classifier();

private:
	const static int m_NumClass = 10;
	int m_NumImgRow, m_NumImgCol; 

	Mat m_TrainImg; 
	vector<uint8_t> m_TrainLbl;
	int m_NumTrainSample; 

	Mat m_TestImg; 
	vector<uint8_t> m_TestLbl;
	int m_NumTestSample; 
};

#endif
