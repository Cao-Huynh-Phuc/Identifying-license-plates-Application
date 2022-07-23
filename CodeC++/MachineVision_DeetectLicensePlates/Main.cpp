//#include "stdafx.h"
#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/ml.hpp>
#include <iostream>
#include <stdio.h>
//#include <Windows.h>
#include "dirent.h"
#include "feature.h"
#include "PlateFinder.h"
#include "Recognise.h"
#include "PlateFinder_Cascade.h"
#include <vector>
#include "Header.h"
//#include "jackylib.h"

using namespace std;
using namespace cv;
using namespace cv::ml;
//using namespace 

int i_img = 0;
int car_plate = 0;




char character_recognition(Mat img_character)
{
	//Load SVM training file OpenCV 3.1
	Ptr<cv::ml::SVM> svmNew = cv::ml::SVM::create();
	//svmNew = SVM::load<SVM>("svm.txt");
	svmNew = cv::ml::SVM::load("svm.txt");

	//SVM svmNew;
	//svmNew.load("D:/svm.txt");

	char c = '*';

	vector<float> feature = calculate_feature(img_character);
	// Open CV3.1
	Mat m = Mat(1, number_of_feature, CV_32FC1);
	for (size_t i = 0; i < feature.size(); ++i)
	{
		float temp = feature[i];
		m.at<float>(0, i) = temp;
	}
	//Mat m = Mat(number_of_feature,1, CV_32FC1);		// Open CV 2.4
	//for (size_t i = 0; i < feature.size(); ++i)
	//{
	//float temp = feature[i];
	//m.at<float>(i,0) = temp;
	//}
	int ri = int(svmNew->predict(m)); // Open CV 3.1
									  /*int ri = int(svmNew.predict(m));*/
	if (ri >= 0 && ri <= 9)
		c = (char)(ri + 48); //ma ascii 0 = 48
	if (ri >= 10 && ri < 18)
		c = (char)(ri + 55); //ma accii A = 5, --> tu A-H
	if (ri >= 18 && ri < 22)
		c = (char)(ri + 55 + 2); //K-N, bo I,J
	if (ri == 22) c = 'P';
	if (ri == 23) c = 'S';
	if (ri >= 24 && ri < 27)
		c = (char)(ri + 60); //T-V,  
	if (ri >= 27 && ri < 30)
		c = (char)(ri + 61); //X-Z

	return c;

}

void imadjust(const Mat1b& src, Mat1b& dst, int tol = 1, Vec2i in = Vec2i(0, 255), Vec2i out = Vec2i(0, 255))
{
	// src : input CV_8UC1 image
	// dst : output CV_8UC1 imge
	// tol : tolerance, from 0 to 100.
	// in  : src image bounds
	// out : dst image buonds

	dst = src.clone();

	tol = max(0, min(100, tol));

	if (tol > 0)
	{
		// Compute in and out limits

		// Histogram
		vector<int> hist(256, 0);
		for (int r = 0; r < src.rows; ++r) {
			for (int c = 0; c < src.cols; ++c) {
				hist[src(r, c)]++;
			}
		}

		// Cumulative histogram
		vector<int> cum = hist;
		for (int i = 1; i < hist.size(); ++i) {
			cum[i] = cum[i - 1] + hist[i];
		}

		// Compute bounds
		int total = src.rows * src.cols;
		int low_bound = total * tol / 100;
		int upp_bound = total * (100 - tol) / 100;
		in[0] = distance(cum.begin(), lower_bound(cum.begin(), cum.end(), low_bound));
		in[1] = distance(cum.begin(), lower_bound(cum.begin(), cum.end(), upp_bound));

	}

	// Stretching
	float scale = float(out[1] - out[0]) / float(in[1] - in[0]);
	for (int r = 0; r < dst.rows; ++r)
	{
		for (int c = 0; c < dst.cols; ++c)
		{
			int vs = max(src(r, c) - in[0], 0);
			int vd = min(int(vs * scale + 0.5f) + out[0], out[1]);
			dst(r, c) = saturate_cast<uchar>(vd);
		}
	}
}
template<typename T>
class myvector : public std::vector<T> {
public:
	const T& operator[](std::size_t _Pos) const {
		std::cout << "debug:" << _Pos << ' ' << this->size() << '\n';
		return (*(this->_Myfirst() + _Pos));
	}
};
int main()
{
	int cnt_img_fail = 0;
	//while (i_img <= 83)
	//{
	    car_plate = 0;
		cv::Mat srcImg;
		//srcImg = imread("Data/sample" + to_string(i_img) + ".jpg");
		srcImg = imread("Biensoxe5.jpg");
		imshow("src", srcImg);
		vector<Mat> plates;
		Mat plate;
		Mat image = srcImg;
		PlateFinder pf;
		plates = pf.FindPlate(srcImg);
		if (plates.size() == 0)
		{
			plates = pf.FindPlate_threshold(srcImg);
		}
		if (plates.size() == 0)
		{
			plates = pf.FindPlate_Cascade(srcImg);
		}
		if (plates.size() == 0)
		{
			car_plate = 1;
			resize(srcImg, srcImg, Size(800, 600));
		    plates = pf.FindPlate_Car(srcImg);
		}
		if (plates.size() == 0)
		{
			car_plate = 1;
			plates = pf.FindPlate_threshold_Car(srcImg);
		}
		if (plates.size() == 0)
		{
			plates = pf.FindPlate_Cascade_Car(srcImg);
		}
		if (plates.size() == 0)
		{
			cout << "Khong nhan dien duoc bien so xe" << '\n';
			cnt_img_fail++;
		}
		else
		{
			plate = plates[0];
			imshow("Test Img", plate);
			Recognise rc;
			rc.detectCharacter(plate);
			putText(srcImg, rc.result.str(), Point(0, srcImg.size[0] - 20), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 255), 2);
			//imshow("Detected Face", image);	

		}
		cout << "Hinh hien tai: " << i_img << '\n';
		cout << "car plate: " << car_plate << endl;
		i_img++;
		time_t hientai = time(0);
		//char* dt = ctime(&hientai);
		string dt = ctime(&hientai);
		dt.erase(dt.length() - 1);
		//char* datetime;
		

		cout << "Date va Time dang local la: " << dt << endl;
		putText(srcImg, dt, Point(0, srcImg.size[0]), FONT_HERSHEY_COMPLEX, 0.75, Scalar(0, 255, 255), 2);
		imshow("Result", srcImg);
		waitKey(0);
		srcImg.release();
		plate.release();
		image.release();
		plates.clear();
	//}
	cout << "So hinh khong phat hien duoc: " << cnt_img_fail << '\n';
	waitKey(0);
	return 0;


}





