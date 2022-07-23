#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/ml.hpp>
#include <vector>
#include "dirent.h"
#include "feature.h"
#include "Header.h"
using namespace std;
using namespace cv;


class Recognise
{
public:
	stringstream result;
	Recognise();
	virtual ~Recognise();
	vector<Mat> FindCharacter(Mat plate);
	void detectCharacter(Mat plate);
	char character_recognition(Mat img_character);
	void imadjust(const Mat1b& src, Mat1b& dst, int tol, Vec2i in, Vec2i out);
	Mat histogram_equalization(Mat src);
	Mat histogram_equalization_hsv(Mat src);
};

