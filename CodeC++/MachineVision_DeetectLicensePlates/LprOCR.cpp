
#include "LprOCR.h"
#include "dirent.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <string>
#include <vector>
#include "feature.h"

using namespace std;
using namespace cv;

LprOCR::LprOCR(void)
{
	
}

LprOCR::LprOCR(Mat _image)
{
	
}
LprOCR::~LprOCR(void)
{

}

bool LprOCR::set_image(Mat _image)
{
	this->image = _image;
	if(image.empty()) 
		return false;
	else
		return true;
}

void LprOCR::load_svm_model(string file_name)
{
	this->svm.load(file_name.c_str());
	ready = true;
}

char LprOCR::get_character()
{
	char c = '*';
	if(this->image.empty()) return c;
	if(!ready) return c;

	vector<float> feature = calculate_feature(image);
	Mat m = Mat(number_of_feature, 1, CV_32FC1);
	for(size_t i = 0; i < feature.size(); ++i)
		m.at<float>(i, 0) = feature[i];
	float r = this->svm.predict(m);
	int ri = (int)r;
	if(ri >= 0 && ri <= 9)
		c = (char)(ri + 48); //ma ascii 0 = 48
	if(ri >= 10 && ri < 18)
		c = (char)(ri + 55); //ma accii A = 5, --> tu A-H
	if(ri >= 18 && ri < 22)
		c = (char)(ri + 55 + 2); //K-N, bo I,J
	if(ri == 22) c = 'P';
	if(ri == 23) c = 'S';
	if(c >= 24 && ri < 27)
		c = (char)(ri + 60); //T-V,  
	if(ri >= 27 && ri < 30)
		c = (char)(ri + 61); //X-Z
	return c;
}

