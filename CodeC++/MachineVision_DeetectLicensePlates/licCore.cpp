#include "licCore.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <vector>
#include "feature.h"



using namespace std;
using namespace cv;

licCore::licCore(void)
{
	done = false;
}

void licCore::set_svm_model(string file_name)
{
	this->svm.load(file_name.c_str());
	ready = true;
}
void licCore::set_image(Mat _image)
{
	this->image = _image;
}

bool licCore::plate_detection()
{
	if (image.empty()) return false;
	double t = (double)cvGetTickCount();
	Mat gray, binary;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	cvtColor(image, gray, CV_BGR2GRAY);
	adaptiveThreshold(gray, binary, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 55, 5);
	Mat or_binary = binary.clone();
	//Mat element = getStructuringElement(MORPH_RECT, Size(5,5), Point(2,2));
	//erode(binary, binary, element);
	findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	if (contours.size() <= 0) return false;
	for (size_t i = 0; i < contours.size(); ++i)
	{
		Rect r = boundingRect(contours.at(i));
		if (r.width > image.cols / 2 || r.height > image.cols / 2 || r.width < 120 || r.height < 20
			|| (double)r.width / r.height > 4.5 || (double)r.width / r.height < 3.5)
			continue;
		Mat sub_binary = or_binary(r);
		Mat _plate = sub_binary.clone();
		vector<vector<Point> > sub_contours;
		vector<Vec4i> sub_hierarchy;
		findContours(sub_binary, sub_contours, sub_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		if (sub_contours.size() < 8) continue;
		int count = 0;
		vector<Mat> c;
		Mat sub_image = image(r);
		vector<Rect> r_characters;
		for (size_t j = 0; j < sub_contours.size(); ++j)
		{
			Rect sub_r = boundingRect(sub_contours.at(j));
			if (sub_r.height > r.height / 2 && sub_r.width < r.width / 8 && sub_r.width > 5 && r.width > 15 && sub_r.x > 5)
			{
				Mat cj = _plate(sub_r);
				double ratio = (double)count_pixel(cj) / (cj.cols*cj.rows);
				if (ratio > 0.2 && ratio < 0.7)
				{
					r_characters.push_back(sub_r);
					rectangle(sub_image, sub_r, Scalar(0, 0, 255), 2, 8, 0);
				}
			}
		}
		if (r_characters.size() >= 7)
		{
			// sap xep 
			for (int i = 0; i < r_characters.size() - 1; ++i)
			{
				for (int j = i + 1; j < r_characters.size(); ++j)
				{
					Rect temp;
					if (r_characters.at(j).x < r_characters.at(i).x)
					{
						temp = r_characters.at(j);
						r_characters.at(j) = r_characters.at(i);
						r_characters.at(i) = temp;
					}
				}
			}
			for (int i = 0; i < r_characters.size(); ++i)
			{
				Mat cj = _plate(r_characters.at(i));
				c.push_back(cj);
			}
			characters.push_back(c);
			sub_binary = or_binary(r);
			plates.push_back(_plate);
			draw_character.push_back(sub_image);
		}
	}

	t = (double)cvGetTickCount() - t;
	t = (double)t / (cvGetTickFrequency()*1000.);
	process_time.push_back(t);
	return true;
}

bool licCore::plate_recognition()
{
	if (plates.size() <= 0) return false;
	double t = (double)cvGetTickCount();

	for (size_t i = 0; i < characters.size(); ++i)
	{
		string result;
		for (size_t j = 0; j < characters.at(i).size(); ++j)
		{
			char cs = character_recognition(characters.at(i).at(j));
			result.push_back(cs);
		}

		this->text_recognition.push_back(result);
	}

	t = (double)cvGetTickCount() - t;
	t = (double)t / (cvGetTickFrequency()*1000.); //convert to second
	process_time.push_back(t);

	return true;
}

char licCore::character_recognition(Mat img_character)
{
	char c = '*';
	if (img_character.empty()) return c;
	if (!ready) return c;

	vector<float> feature = calculate_feature(img_character);
	Mat m = Mat(number_of_feature, 1, CV_32FC1);
	for (size_t i = 0; i < feature.size(); ++i)
		m.at<float>(i, 0) = feature[i];
	float r = this->svm.predict(m);
	int ri = (int)r;
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

void licCore::do_process()
{
	if (this->plate_detection() && this->plate_recognition())
		done = true;
}

vector<string> licCore::get_text_recognition()
{
	if (done)
		return this->text_recognition;
}

vector<Mat> licCore::get_plate_detection()
{
	if (done)
		return this->plates;
}

vector<Mat> licCore::get_character_isolation()
{
	if (done)
		return this->draw_character;
}

vector<vector<Mat> > licCore::get_character()
{
	if (done)
		return this->characters;
}

void licCore::sort_character(vector<Mat>& ch)
{
	if (ch.size() < 2) return;
	for (size_t i = 0; i < ch.size() - 1; ++i)
		for (size_t j = i + 1; j < ch.size(); ++j)
		{
			//
		}
}

vector<double> licCore::get_process_time()
{
	return process_time;
}

void licCore::clear()
{
	this->characters.clear();
	this->draw_character.clear();
	this->plates.clear();
	this->text_recognition.clear();
	this->process_time.clear();
}