#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>

#include "dirent.h"

using namespace std;
using namespace cv;

class LprOCR
{
public:
	LprOCR(void);
	LprOCR(Mat);
	~LprOCR(void);
	bool set_image(Mat);
	void load_svm_model(string);
	char get_character();

private:
	bool ready;
	Mat image;
	SVM svm;
};

