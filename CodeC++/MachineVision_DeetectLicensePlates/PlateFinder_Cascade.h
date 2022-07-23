#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/ml.hpp>
using namespace cv;
using namespace std;

class PlateFinder_Cascade
{
public:
	PlateFinder_Cascade();
	virtual ~PlateFinder_Cascade();
	vector<Mat> FindPlate(Mat src);
	int CountCharacter(Mat plate);
};

