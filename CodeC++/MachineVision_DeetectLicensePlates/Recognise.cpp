#include "Recognise.h"

const Scalar RED = Scalar(0, 0, 255);
const Scalar GREEN = Scalar(0, 255, 0);
const Scalar BLUE = Scalar(255, 0, 0);


Recognise::Recognise()
{
}


Recognise::~Recognise()
{
}

vector<Mat> Recognise::FindCharacter(Mat plate) {
	vector<Mat> charImgVector;
	vector<Rect> rects;
	Mat resizeImg, binaryImg, cloneImg;

	//resizeImg = cvCreateImage(cvSize(408, 70), IPL_DEPTH_8U, 3);
	//binaryImg = cvCreateImage(cvSize(408, 70), IPL_DEPTH_8U, 1);
	int w = plate.size[1];
	int h = plate.size[0];
	if (car_plate == 0)
		resize(plate, resizeImg, Size(w * 2, round(h * 2.0)));
	else
		resize(plate, resizeImg, Size(408, 70));
	//resize(plate, resizeImg, Size(w * 2, round(h * 2.0)));

	//cout << "wxh" << w*2 << "x" << h*2 <<'\n';
	//resizeImg = histogram_equalization(resizeImg);
	//medianBlur(resizeImg, resizeImg, 3);
	//cvResize(plate, resizeImg);
	
	cvtColor(resizeImg, binaryImg, CV_RGB2GRAY);

	adaptiveThreshold(binaryImg, binaryImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 13, 7);
	
	//threshold(binaryImg, binaryImg, (int)mean(binaryImg).val[0], 255, THRESH_BINARY);
	Mat element = getStructuringElement(MORPH_RECT, cv::Size(3, 3));
	if (car_plate == 0)
	{
		morphologyEx(binaryImg, binaryImg, MORPH_CLOSE, element, Point(-1, -1), 1);
		//Mat element3 = getStructuringElement(MORPH_RECT, cv::Size(2, 2));
		erode(binaryImg, binaryImg, element, Point(-1, -1), 1);
		dilate(binaryImg, binaryImg, element, Point(-1, -1), 1);
		//Mat element1 = getStructuringElement(MORPH_RECT, cv::Size(1, 3));
		//erode(binaryImg, binaryImg, element1, Point(-1, -1), 1);
		//Mat element2 = getStructuringElement(MORPH_RECT, cv::Size(3, 1));
		//erode(binaryImg, binaryImg, element, Point(-1, -1), 1);
	}
    
	

	//imwrite("Plates/plate_gray" + to_string(i_img) + ".jpg", binaryImg);
	imshow("binaryImg", binaryImg);
	cloneImg = resizeImg.clone();
	bitwise_not(binaryImg, binaryImg);
	
	//imshow("binaryImg", binaryImg);
	//cout << "done" << '\n';

	vector<vector<cv::Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(binaryImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	double ratio, ratioWhite;
	int s, white;
	int smin;
	double ratio_max, ratio_min, ratioWhite_max, ratioWhite_min;
	double w_max, w_min, h_max, h_min;

	if (car_plate == 0)
	{
		smin = w*4.0*h*0.015;
		ratio_max = 1.1;
		ratio_min = 0.2;
		w_min = w*2.0*0.045;
		w_max = w*2.0 / 4;
		h_max = h*2.0*0.53;
		h_min = h*2.0*0.3;
		ratioWhite_max = 0.75;
		ratioWhite_min = 0.3;
	}
	else
	{
		smin = 375;
		ratio_max = 1.1;
		ratio_min = 0.3;
		w_min = 15;
		w_max = 50;
		h_min = 30;
		h_max = 70;
		ratioWhite_max = 0.75;
		ratioWhite_min = 0.3;
	}

	if (contours.size() != 0)
	{
		for (size_t i = 0; i < contours.size(); i++)
		{
			Rect rect = boundingRect(contours.at(i));
			rectangle(cloneImg, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), RED, 2);
			//cout << rect.width << "x" << rect.height << '\n';
			ratio = (double)rect.width / rect.height;
			s = rect.width * rect.height;

			//cvSetImageROI(binaryImg, r);
			white = countNonZero(binaryImg(rect));
			//cvResetImageROI(binaryImg);
			ratioWhite = (double)white / s;

			if (ratio > 0.2 && ratio < 1.1 &&
				s > smin && rect.width > w_min && rect.width < w_max &&
				rect.height > h_min && rect.height < h_max &&
				ratioWhite > ratioWhite_min && ratioWhite < ratioWhite_max) //&& rect.x > 2)
			{
				rectangle(cloneImg, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), GREEN, 2);

				rects.push_back(rect);
			}

			//contours = contours->h_next;
		}
		//imwrite("Plates/plate" + to_string(i_img) + ".jpg", cloneImg);

		// sap xep
		//cout << rects.size() << '\n';
		if (rects.size() != 0)
		{
			vector<Rect> character;
			int upline = 0;
			for (int i = 0; i < rects.size(); i++)
			{
				if (rects[i].y < resizeImg.size[0] * 0.3)
					character.push_back(rects[i]);
			}
			if (character.size() > 1)
			{
				for (int i = 0; i < character.size() - 1; i++)
				{
					for (int j = i + 1; j < character.size(); j++)
					{
						if (character[i].x > character[j].x)
						{
							Rect sw;
							sw = character[i];
							character[i] = character[j];
							character[j] = sw;
						}
					}
				}
				upline = character.size();
			}
			
			for (int i = 0; i < rects.size(); i++)
			{
				if (rects[i].y >= resizeImg.size[0] * 0.3)
					character.push_back(rects[i]);
			}
			if (character.size() > upline + 1)
			{
				for (int i = upline; i < character.size() - 1; i++)
				{
					for (int j = i + 1; j < character.size(); j++)
					{
						if (character[i].x > character[j].x)
						{
							Rect sw;
							sw = character[i];
							character[i] = character[j];
							character[j] = sw;
						}
					}
				}
			}
			

			// cat ky tu
			Mat charImg;
			Mat saveImg;

			for (int i = 0; i < character.size(); i++)
			{
				//charImg = cvCreateImage(cvSize(rect[i].width, rect[i].height), IPL_DEPTH_8U, 3);
				//cvSetImageROI(resizeImg, rect[i]);
				//cvCopy(resizeImg, charImg, NULL);
				//cvResetImageROI(resizeImg);
				charImg = resizeImg(character[i]);

				// add anh vao vector ki tu
				charImgVector.push_back(charImg);

				// show
				char name[8];
				//sprintf(name, "Anh %d", i + 1);
				//imshow(name, charImg);


				// Luu anh lam mau de training
				//saveImg = cvCreateImage(cvSize(rect[i].width, rect[i].height), IPL_DEPTH_8U, 1);
				cvtColor(charImgVector[i], saveImg, CV_RGB2GRAY);
				//adaptiveThreshold(saveImg, saveImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 13, 2);
				threshold(saveImg, saveImg, (int)mean(saveImg).val[0], 255, THRESH_BINARY);
				//charImgVector.push_back(saveImg);
				Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(0, 0));
				//dilate(saveImg, saveImg, kernel, Point(-1, -1), 1);
				//erode(saveImg, saveImg, kernel, Point(-1, -1), 1);
				//dilate(saveImg, saveImg, kernel, Point(-1, -1), 1);
				//erode(saveImg, saveImg, kernel, Point(-1, -1), 1);

				//imshow(name, charImg);
				//char sname[8];
				sprintf_s(name, "Anh %d", i + 1);
				//imwrite("Data/sample" + to_string(i_img) + "/digit_" + to_string(i_img) + "_" + to_string(i) + ".jpg", saveImg);

				imshow(name, saveImg);
				//sprintf(sname, "Data/%d.jpg", i + 1);
				//cvShowImage(sname, saveImg);
				saveImg.release();
				charImg.release();
				//cvSaveImage(sname, saveImg);
			}

		}
				
	}
	imshow("character", cloneImg);
	resizeImg.release();
	cloneImg.release();
	binaryImg.release();
	element.release();

	return charImgVector;
}

void Recognise::detectCharacter(Mat plate) {
	
	vector<Mat> characterVector = FindCharacter(plate);
	
	if (characterVector.size() == 0)
	{
		cout << "Khong phat hien ky tu" << '\n';
	}
	else
	{
		
		Mat grayImg;
		
		//Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(0, 0));

		for (size_t i = 0; i < characterVector.size(); i++)
		{
			cvtColor(characterVector[i], grayImg, CV_RGB2GRAY);
			//normalize(grayImg, grayImg, 0, 255, NORM_MINMAX);
			//adaptiveThreshold(grayImg, grayImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 5, 2);
			threshold(grayImg, grayImg, (int)mean(grayImg).val[0], 255, THRESH_BINARY);
			//dilate(grayImg, grayImg, kernel, Point(-1, -1), 1);
			//erode(grayImg, grayImg, kernel, Point(-1, -1), 1);
			//imwrite("Data/sample" + to_string(i_img) + "/digit_" + to_string(i_img) + "_" + to_string(i) + ".jpg", grayImg);
			//for (size_t j = 0; j < characterVector.size(); ++j)
			//{

			char cs = character_recognition(grayImg);
			if (car_plate == 0)
			{
				if ((i >= 4) || (i < 2))
				{
					if (cs == 'B')
						cs = '8';
					if (cs == 'D')
						cs = '0';
					if (cs == 'S')
						cs = '5';
				}
			}
			else
			{
				if (i != 2)
				{
					if (cs == 'B')
						cs = '8';
					if (cs == 'D')
						cs = '0';
					if (cs == 'S')
						cs = '5';
				}
			}
			
			//result.push_back(cs);
			//stringstream str;

			result << cs;
			cout << cs << endl;
			//cout << ((stringstream)cs).str() << endl;
			//stringstream str;
			//str << cs;

			//putText(image, str.str(), Point(r_characters.at(j).x + r.x, r_characters.at(j).y + r.y), FONT_HERSHEY_COMPLEX, 0.75, Scalar(0, 255, 0), 1);

			//}
			//text_recognition.push_back(result);
			grayImg.release();

		}

	}
	characterVector.clear();
	

}

char Recognise::character_recognition(Mat img_character)
{
	//Load SVM training file OpenCV 3.1
	Ptr<cv::ml::SVM> svmNew = cv::ml::SVM::create();
	//svmNew = SVM::load<SVM>("svm.txt");
	if (car_plate == 0)
		svmNew = cv::ml::SVM::load("svm.txt");
	else
		svmNew = cv::ml::SVM::load("svm_car.txt");

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
	if (car_plate == 0)
	{
		if (ri == 31) c = 'R';
	}
	else 
	{
		if (ri == 30) c = 'W';
		if (ri == 31) c = 'O';
		if (ri == 32) c = 'I';
	}
	
	return c;

}


void Recognise::imadjust(const Mat1b& src, Mat1b& dst, int tol = 1, Vec2i in = Vec2i(0, 255), Vec2i out = Vec2i(0, 255))
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


Mat Recognise::histogram_equalization(Mat src) {
	Mat dst;
	//cvtColor(image, img, COLOR_BGR2HSV);
	vector<Mat> bgr_channel(3);
	Mat1b b, g, r;
	split(src, bgr_channel);
	b = bgr_channel[0];
	g = bgr_channel[1];
	r = bgr_channel[2];

	imadjust(b, b);
	imadjust(g, g);
	imadjust(r, r);
	//v = v*1.2;


	bgr_channel[0] = b;
	bgr_channel[1] = g;
	bgr_channel[2] = r;
	merge(bgr_channel, dst);
	//cvtColor(img, img, COLOR_HSV2BGR);
	b.release();
	g.release();
	r.release();
	bgr_channel.clear();

	return dst;
}


Mat Recognise::histogram_equalization_hsv(Mat src) {
	Mat dst;
	Mat hsvImg;
	cvtColor(src, hsvImg, COLOR_BGR2HSV);
	vector<Mat> hsv_channel(3);
	Mat1b h, s, v;
	split(src, hsv_channel);
	h = hsv_channel[0];
	s = hsv_channel[1];
	v = hsv_channel[2];

	//imadjust(h, h);
	//imadjust(s, s);
	//imadjust(v, v);
	v = v*1.2;


	hsv_channel[0] = h;
	hsv_channel[1] = s;
	hsv_channel[2] = v;
	merge(hsv_channel, dst);
	cvtColor(dst, dst, COLOR_HSV2BGR);
	h.release();
	s.release();
	v.release();
	hsvImg.release();
	hsv_channel.clear();
	return dst;
}