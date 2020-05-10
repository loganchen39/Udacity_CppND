#include "opencv2/opencv.hpp"
#include "mnistdb.h"

#include <string>

using namespace cv;
using namespace std;


int main()
{
	Mnistdb dataset;
    string DIR_DATA = "/home/workspace/data";

	dataset.ReadTrainImg(DIR_DATA + "/train-images.idx3-ubyte");
	dataset.ReadTrainLbl(DIR_DATA + "/train-labels.idx1-ubyte");
	dataset.ReadTestImg(DIR_DATA + "/t10k-images.idx3-ubyte");
	dataset.ReadTestLbl(DIR_DATA + "/t10k-labels.idx1-ubyte");
 
  
    dataset.SVM_Classifier();

	return 0;
}