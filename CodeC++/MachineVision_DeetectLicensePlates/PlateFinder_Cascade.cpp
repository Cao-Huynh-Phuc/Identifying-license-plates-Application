#include "PlateFinder_Cascade.h"
const Scalar RED = Scalar(0, 0, 255);
const Scalar GREEN = Scalar(0, 255, 0);
const Scalar BLUE = Scalar(255, 0, 0);


PlateFinder_Cascade::PlateFinder_Cascade()
{
}


PlateFinder_Cascade::~PlateFinder_Cascade()
{
}


vector<Mat> PlateFinder_Cascade::FindPlate(Mat src) {
	// Detect LP
	vector<Mat> plates;
	vector<Rect> Licence_plate;
	vector<Rect>::iterator abc = Licence_plate.begin();
	Mat src_clone = src.clone();
	// Load LP Localization cascade (.xml file)
	CascadeClassifier LPL_cascade;
	LPL_cascade.load("output-hv-33-x25.xml");
	//LPL_cascade.load("vn2.xml");
	//LPL_cascade.load("au.xml");
	LPL_cascade.detectMultiScale(src, Licence_plate, 1.1, 3, 0 | CV_HAAR_FIND_BIGGEST_OBJECT, Size(10, 10));
	if (Licence_plate.empty())
	{
		cout << "using au.xml  for car licence pltae detect" << "\n";
		LPL_cascade.load("au.xml");
		LPL_cascade.detectMultiScale(src, Licence_plate, 1.1, 3, 0 | CV_HAAR_FIND_BIGGEST_OBJECT, Size(10, 10));
	}
	if (!Licence_plate.empty())
	{
		for (size_t i = 0; i < Licence_plate.size(); i++)
		{
			int cnt = CountCharacter(src(Licence_plate[i]));
			if (cnt >= 5) {
				//plateArr[j] = cvCloneImage(plate);
				//j++;
				plates.push_back(src(Licence_plate[i]));

			}
			//plates.push_back(src(Licence_plate[i]));
			rectangle(src_clone, Licence_plate[i], Scalar(0, 255, 0), 1);

		}		
	}
	
	imshow("test", src_clone);

	return plates;
}


int PlateFinder_Cascade::CountCharacter(Mat plate) {
	int cnt = 0;
	Mat resizeImg, binaryImg;
	int w = plate.size[1];
	int h = plate.size[0];
	//resizeImg = Mat(Size(130, 110), plate.depth, 3);
	//binaryImg = Mat(Size(130, 110), plate.depth, 1);

	resize(plate, resizeImg, Size(w * 2, h * 2));
	cout << w << " " << h << '\n';
	imshow("resizeImg1", resizeImg);
	cvtColor(resizeImg, binaryImg, COLOR_RGB2GRAY);
	adaptiveThreshold(binaryImg, binaryImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 13, 2);
	//threshold(binaryImg, binaryImg, (int)mean(binaryImg).val[0], 255, THRESH_BINARY);
	bitwise_not(binaryImg, binaryImg);
	imshow("binaryImg", binaryImg);

	//cvShowImage("binaryImg", binaryImg);

	vector<vector<cv::Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(binaryImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	//cvShowImage("contours", binaryImg);

	//CvSeq *contours = 0;
	//cvFindContours(binaryImg, storage, &contours);

	for (size_t i = 0; i < contours.size(); i++)
	{
		Rect rect = boundingRect(contours.at(i));
		rectangle(resizeImg, Point(rect.x, rect.y),
			Point(rect.x + rect.width, rect.y + rect.height), RED, 2);
		//cout << rect.width << " " << rect.height << '\n';
		if (rect.width >(w*2.0*0.045) && rect.width < (w*2.0 / 4)
			&& rect.height >(h*2.0*0.3) && rect.height < (h*2.0*0.5)
			&& rect.width * rect.height >(w*4.0*h*0.015))
		{
			rectangle(resizeImg, Point(rect.x, rect.y),
				Point(rect.x + rect.width, rect.y + rect.height), GREEN, 2);

			cnt++;
		}
	}

	//cvShowImage("resizeImg", resizeImg);
	imshow("resizeImg", resizeImg);
	resizeImg.release();
	binaryImg.release();

	return cnt;
}