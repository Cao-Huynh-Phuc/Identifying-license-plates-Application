#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/ml.hpp>
#include "Header.h"
using namespace cv;
using namespace std;

class PlateFinder
{
private:
	Mat S1;
	Mat S2;
	Mat plate;
public:
	PlateFinder();
	virtual ~PlateFinder();
	Mat ImageRestoration(Mat src);	// tien xu ly anh goc
	void ImageRestoration_Car(Mat src);	// tien xu ly anh goc
	vector<Mat> FindPlate(Mat src);	// tim va cat bien so
	vector<Mat> FindPlate_threshold(Mat src);	// tim va cat bien so
	vector<Mat> FindPlate_Cascade(Mat src);
	vector<Mat> PlateFinder::FindPlate_threshold_Car(Mat src);
	vector<Mat> FindPlate_Car(Mat src);
	vector<Mat> FindPlate_Cascade_Car(Mat src);
	int CountCharacter(Mat plate);	// dem so vung co kha nang la ki tu

};

