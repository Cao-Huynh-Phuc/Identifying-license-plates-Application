#include "PlateFinder.h"
#include <iostream>
using namespace std;

const Scalar RED = Scalar(0, 0, 255);
const Scalar GREEN = Scalar(0, 255, 0);
const Scalar BLUE = Scalar(255, 0, 0);


PlateFinder::PlateFinder()
{
	S1 = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 0));
	S2 = getStructuringElement(MORPH_RECT, Size(6, 1), Point(3, 0));
	//plate = NULL;
}


PlateFinder::~PlateFinder()
{
	
}


Mat PlateFinder::ImageRestoration(Mat src)
{
	int w = src.size[1];
	int h = src.size[0];

	Mat mImg; 		// Anh su dung cho bien doi hinh thai hoc
	Mat src_pyrdown;
	//Mat tmp;
	Mat thresholed;	// Anh nhi phan voi nguong
	Mat mini_thresh;
	Mat dst = Mat::zeros(src.size(), CV_8UC1);			// Anh lam ro vung bien so

	//cvPyrDown(src, src_pyrdown); //Giu lai cac dac trung cua anh sau resize(w/2,h/2)
	src_pyrdown = src.clone();
	//mImg = src.clone();
	//imshow("src_pyrdown", src_pyrdown);
	morphologyEx(src_pyrdown, mImg, MORPH_BLACKHAT, S2);
	//imshow("mImg", mImg);
	//cvtColor(src_pyrdown, mImg, COLOR_BGR2GRAY);
	normalize(mImg, mImg, 0, 255, NORM_MINMAX);
	imshow("mImg1", mImg);

	
	// Nhi phan hoa anh mImg
	//cout << (int)mean(mImg).val[0] << endl;
	threshold(mImg, thresholed, (int)10*mean(mImg).val[0], 255, THRESH_BINARY); //Can nhan 10 cho mean(mImg).val[0] có the do normalize
	imshow("threshold", thresholed);
	//cvZero(dst);
	thresholed.copyTo(mini_thresh);
	//cout << mini_thresh.size[0] << endl;

	// Su dung hinh chu nhat co size = 8x16 truot tren toan bo anh

	int cnt;
	int nonZero1, nonZero2, nonZero3, nonZero4;
	Rect rect;//=Rect(400, 300, 16, 16);
	//nonZero1 = countNonZero(mini_thresh(rect));
	//cout << nonZero1 << endl;

	for (int i = 0; i < mini_thresh.size[1] - 16; i += 4)
	{
		for (int j = 0; j < mini_thresh.size[0] - 16; j += 4)
		{
			rect = Rect(i, j, 8, 8);
			//cvSetImageROI(mini_thresh, rect);	//ROI = Region of Interest	
			nonZero1 = countNonZero(mini_thresh(rect)); //ROI = Region of Interest
			//cvResetImageROI(mini_thresh);

			rect = Rect(i + 8, j, 8, 8);
			//cvSetImageROI(mini_thresh, rect);	//ROI = Region of Interest
			nonZero2 = countNonZero(mini_thresh(rect));
			//cvResetImageROI(mini_thresh);

			rect = Rect(i, j + 8, 8, 8);
			//cvSetImageROI(mini_thresh, rect);	//ROI = Region of Interest
			nonZero3 = countNonZero(mini_thresh(rect));
			//cvResetImageROI(mini_thresh);

			rect = Rect(i + 8, j + 8, 8, 8);
			//cvSetImageROI(mini_thresh, rect);	//ROI = Region of Interest
			nonZero4 = countNonZero(mini_thresh(rect));
			//cvResetImageROI(mini_thresh);

			cnt = 0;
			if (nonZero1 > 8) { cnt++; }
			if (nonZero2 > 8) { cnt++; }
			if (nonZero3 > 8) { cnt++; }
			if (nonZero4 > 8) { cnt++; }

			if (cnt > 2)
			{
				rect = Rect(i, j, 16, 16);
				/*cvSetImageROI(dst, rect);
				cvSetImageROI(mini_thresh, rect);
				cvCopy(mini_thresh, dst);
				cvResetImageROI(dst);
				cvResetImageROI(mini_thresh);*/
				mini_thresh(rect).copyTo(dst(rect));

			}
		}
	}

	Mat dst_clone = dst.clone();
	imshow("dst_clone", dst_clone);
	dilate(dst, dst, Mat(), Point(-1,-1), 2);
	erode(dst, dst, Mat(), Point(-1, -1), 2);
	dilate(dst, dst, S1, Point(-1, -1), 7);
	erode(dst, dst, S1, Point(-1, -1), 8);
	dilate(dst, dst, Mat());
	imshow("dst", dst);


	mini_thresh.release();
	mImg.release();
	src_pyrdown.release();
	thresholed.release();

	return dst;
}


void PlateFinder::ImageRestoration_Car(Mat src)
{
	int w = src.size[1];
	int h = src.size[0];

	Mat mImg; 		// Anh su dung cho bien doi hinh thai hoc
	Mat src_pyrdown = Mat(Size(400, 300), CV_8UC1, 1);
	
	//Mat tmp;
	pyrDown(src, src_pyrdown);
	//imshow("abc", src_pyrdown);
	Mat thresholed;	// Anh nhi phan voi nguong
	Mat mini_thresh;
	Mat dst = Mat::zeros(src_pyrdown.size(), CV_8UC1);			// Anh lam ro vung bien so

														//cvPyrDown(src, src_pyrdown); //Giu lai cac dac trung cua anh sau resize(w/2,h/2)
	//src_pyrdown = src.clone();
	//mImg = src.clone();
	//imshow("src_pyrdown", src_pyrdown);
	morphologyEx(src_pyrdown, mImg, MORPH_BLACKHAT, S2);
	//imshow("mImg", mImg);
	//cvtColor(src_pyrdown, mImg, COLOR_BGR2GRAY);
	normalize(mImg, mImg, 0, 255, NORM_MINMAX);
	imshow("mImg1", mImg);


	// Nhi phan hoa anh mImg
	//cout << (int)mean(mImg).val[0] << endl;
	threshold(mImg, thresholed, (int)10 * mean(mImg).val[0], 255, THRESH_BINARY); //Can nhan 10 cho mean(mImg).val[0] có the do normalize
	imshow("threshold", thresholed);
	//cvZero(dst);
	thresholed.copyTo(mini_thresh);
	//cout << mini_thresh.size[0] << endl;

	// Su dung hinh chu nhat co size = 8x16 truot tren toan bo anh

	int cnt;
	int nonZero1, nonZero2, nonZero3, nonZero4;
	Rect rect;//=Rect(400, 300, 16, 16);
			  //nonZero1 = countNonZero(mini_thresh(rect));
			  //cout << nonZero1 << endl;

	for (int i = 0; i < mini_thresh.size[1] - 32; i += 4)
	{
		for (int j = 0; j < mini_thresh.size[0] - 16; j += 4)
		{
			rect = Rect(i, j, 16, 8);
			//cvSetImageROI(mini_thresh, rect);	//ROI = Region of Interest	
			nonZero1 = countNonZero(mini_thresh(rect)); //ROI = Region of Interest
														//cvResetImageROI(mini_thresh);

			rect = Rect(i + 16, j, 16, 8);
			//cvSetImageROI(mini_thresh, rect);	//ROI = Region of Interest
			nonZero2 = countNonZero(mini_thresh(rect));
			//cvResetImageROI(mini_thresh);

			rect = Rect(i, j + 8, 16, 8);
			//cvSetImageROI(mini_thresh, rect);	//ROI = Region of Interest
			nonZero3 = countNonZero(mini_thresh(rect));
			//cvResetImageROI(mini_thresh);

			rect = Rect(i + 16, j + 8, 16, 8);
			//cvSetImageROI(mini_thresh, rect);	//ROI = Region of Interest
			nonZero4 = countNonZero(mini_thresh(rect));
			//cvResetImageROI(mini_thresh);

			cnt = 0;
			if (nonZero1 > 15) { cnt++; }
			if (nonZero2 > 15) { cnt++; }
			if (nonZero3 > 15) { cnt++; }
			if (nonZero4 > 15) { cnt++; }

			if (cnt > 2)
			{
				rect = Rect(i, j, 32, 16);
				/*cvSetImageROI(dst, rect);
				cvSetImageROI(mini_thresh, rect);
				cvCopy(mini_thresh, dst);
				cvResetImageROI(dst);
				cvResetImageROI(mini_thresh);*/
				mini_thresh(rect).copyTo(dst(rect));

			}
		}
	}

	Mat dst_clone = dst.clone();
	imshow("dst_clone", dst_clone);
	dilate(dst, dst, Mat(), Point(-1, -1), 2);
	erode(dst, dst, Mat(), Point(-1, -1), 2);
	dilate(dst, dst, S1, Point(-1, -1), 9);
	erode(dst, dst, S1, Point(-1, -1), 10);
	dilate(dst, dst, Mat());
	imshow("dst", dst);

	/*cvShowImage("Source" , src);
	cvShowImage("mImg", mImg);
	cvShowImage("mini_thresh", mini_thresh);
	cvShowImage("dst_clone", dst_clone);
	cvShowImage("dst", dst);*/

	//cvPyrUp(dst, src);

	/*cvReleaseImage(&mini_thresh);
	cvReleaseImage(&mImg);
	cvReleaseImage(&tmp);
	cvReleaseImage(&dst);
	cvReleaseImage(&src_pyrdown);
	cvReleaseImage(&thresholed);
	cvReleaseImage(&dst_clone);*/
	mini_thresh.release();
	mImg.release();
	src_pyrdown.release();
	thresholed.release();
	pyrUp(dst, src);
	//return dst;
}


vector<Mat> PlateFinder::FindPlate(Mat src) {
	Mat plate;
	vector<Mat> plates;
	Mat contourImg; //= cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);	// anh tim contour
	Mat grayImg;    // anh xam
	cvtColor(src, grayImg, COLOR_RGB2GRAY);

	Mat cloneImg; //= cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
	cloneImg = src.clone();

	// tien xu ly anh
	//cvCopy(grayImg, contourImg);
	grayImg.copyTo(contourImg);
	normalize(contourImg, contourImg, 0, 255, NORM_MINMAX);
	//imshow("contourImg", contourImg);
	contourImg = ImageRestoration(contourImg).clone();
	//imshow("contourImg", contourImg);
	Mat rectImg; //= cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
	src.copyTo(rectImg);
	//cvMerge(contourImg, contourImg, contourImg, NULL, rectImg); // tron anh

    // tim contour cua buc anh

	vector<vector<cv::Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(contourImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	//cvShowImage("contourImg", contourImg);

	int xmin, ymin, xmax, ymax, w, h, s, ratio_plate_src;
	int count;
	double ratio_plate;	// ty le chieu rong tren chieu cao
	Rect rectPlate;


	//cout << contours.size() << '\n';

	for (size_t i = 0; i < contours.size(); i++) 
	{
		/*count = contours->total;
		CvPoint *PointArray = new CvPoint[count];
		cvCvtSeqToArray(contours, PointArray, CV_WHOLE_SEQ);*/

		Rect r_Plate = boundingRect(contours.at(i));
		if (r_Plate.y >= 3)
			r_Plate = Rect(r_Plate.x - 2, r_Plate.y - 3, r_Plate.width - 3, r_Plate.height + 2);
		else
			r_Plate = Rect(r_Plate.x - 2, r_Plate.y, r_Plate.width - 3, r_Plate.height);
		//cout << "test" << r_Plate.y << '\n';
		
		rectangle(cloneImg, r_Plate, GREEN, 2);

		w = r_Plate.width;
		h = r_Plate.height;
		s = w * h;

		//cvRectangle(rectImg, cvPoint(xmin, ymin), cvPoint(xmax, ymax), RED);

		// loai bo nhung hinh chu nhat co ti le khong dung
		if (s != 0) {
			ratio_plate_src = (contourImg.size[0]* contourImg.size[1]) / s;
		}
		else {
			ratio_plate_src = 1000;
		}

		if (h == 0) {
			ratio_plate = 0;
		}
		else {
			ratio_plate = (double)w / h;
		}

		if (ratio_plate_src > 19 && ratio_plate_src < 100) {
			// ve ra hcn mau xanh la
			//cvRectangle(rectImg, cvPoint(xmin, ymin), cvPoint(xmax, ymax), GREEN);

			if (ratio_plate > 1 && ratio_plate < 1.6) {
				//cvRectangle(rectImg, cvPoint(xmin, ymin), cvPoint(xmax, ymax), BLUE);

				if (w > 40 && w < 100 && h > 30 && h < 100) {


					// cat bien so
					//plate = cvCreateImage(cvSize(rectPlate.width, rectPlate.height), IPL_DEPTH_8U, 3);
					
					plate = src(r_Plate);
					rectangle(rectImg, r_Plate, RED, 1);

					// luu vao mang cac bien so plateArr
					int cnt = CountCharacter(plate);
					cout << "cnt" << cnt << '\n';
					if (cnt >= 5) {

						plates.push_back(plate);
						

					}
				}
			}
		}


	}



	//}
	imshow("cloneImg", cloneImg);
	imshow("rectImage", rectImg);
	//waitKey(0);
	contourImg.release();
	cloneImg.release();
	rectImg.release();
	grayImg.release();

	//if (plates.size() != 0)
		return plates;
	//else
	//	return cv::Mat(55, 65, CV_8UC3, GREEN);
}


vector<Mat> PlateFinder::FindPlate_threshold(Mat src){
	Mat plate;
	vector<Mat> plates;
	Mat contourImg; //= cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);	// anh tim contour
	Mat grayImg;    // anh xam
	cvtColor(src, grayImg, COLOR_RGB2GRAY);

	Mat cloneImg; //= cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
	cloneImg = src.clone();

	// tien xu ly anh
	//cvCopy(grayImg, contourImg);
	grayImg.copyTo(contourImg);
	//normalize(contourImg, contourImg, 0, 255, NORM_MINMAX);
	//imshow("contourImg", contourImg);
	//contourImg = ImageRestoration(contourImg).clone();
	threshold(contourImg, contourImg, 180, 255, THRESH_BINARY);
	//Mat element = getStructuringElement(MORPH_RECT, cv::Size(3, 3));
	//dilate(contourImg, contourImg, element, Point(-1, -1), 2);
	//Mat or_binary = binary.clone();
	//erode(contourImg, contourImg, element, Point(-1, -1), 2);
	imshow("contourImg", contourImg);
	Mat rectImg; //= cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
	src.copyTo(rectImg);
	//cvMerge(contourImg, contourImg, contourImg, NULL, rectImg); // tron anh

	// tim contour cua buc anh
	
	vector<vector<cv::Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(contourImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	//cvShowImage("contourImg", contourImg);

	int w, h, s, ratio_plate_src;
	int count;
	double ratio_plate;	// ty le chieu rong tren chieu cao
	Rect rectPlate;



	//cout << contours.size() << '\n';

	for (size_t i = 0; i < contours.size(); i++)
	{
		/*count = contours->total;
		CvPoint *PointArray = new CvPoint[count];
		cvCvtSeqToArray(contours, PointArray, CV_WHOLE_SEQ);*/

		Rect r_Plate = boundingRect(contours.at(i));
		r_Plate = Rect(r_Plate.x, r_Plate.y + 2, r_Plate.width - 3, r_Plate.height - 2);
		rectangle(cloneImg, r_Plate, GREEN, 2);

		w = r_Plate.width;
		h = r_Plate.height;
		s = w * h;

		//cvRectangle(rectImg, cvPoint(xmin, ymin), cvPoint(xmax, ymax), RED);

		// loai bo nhung hinh chu nhat co ti le khong dung
		if (s != 0) {
			ratio_plate_src = (contourImg.size[0] * contourImg.size[1]) / s;
		}
		else {
			ratio_plate_src = 1000;
		}

		if (h == 0) {
			ratio_plate = 0;
		}
		else {
			ratio_plate = (double)w / h;
		}

		if (ratio_plate_src > 20 && ratio_plate_src < 100) {
			// ve ra hcn mau xanh la
			//cvRectangle(rectImg, cvPoint(xmin, ymin), cvPoint(xmax, ymax), GREEN);

			if (ratio_plate > 1 && ratio_plate < 1.6) {
				//cvRectangle(rectImg, cvPoint(xmin, ymin), cvPoint(xmax, ymax), BLUE);

				if (w > 40 && w < 100 && h > 30 && h < 100) {

					plate = src(r_Plate);
					rectangle(rectImg, r_Plate, RED, 1);

					// luu vao mang cac bien so plateArr
					int cnt = CountCharacter(plate);
					if (cnt >= 5) {
						//plateArr[j] = cvCloneImage(plate);
						//j++;
						plates.push_back(plate);

					}
				}
			}
		}


	}

	imshow("cloneImg", cloneImg);
	imshow("rectImage", rectImg);

	contourImg.release();
	cloneImg.release();
	rectImg.release();
	grayImg.release();
	//element.release();

	return plates;

}


vector<Mat> PlateFinder::FindPlate_Cascade(Mat src) {
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


	//if (Licence_plate.empty())
	//{
	//	cout << "using au.xml  for car licence pltae detect" << "\n";
	//	car_plate = 1;
	//	LPL_cascade.load("au.xml");
	//	//LPL_cascade.load("br.xml");
	//	LPL_cascade.detectMultiScale(src, Licence_plate, 1.1, 3, 0 | CV_HAAR_FIND_BIGGEST_OBJECT, Size(10, 10));
	//}



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


vector<Mat> PlateFinder::FindPlate_threshold_Car(Mat src) {
	Mat plate;
	vector<Mat> plates;
	Mat contourImg; //= cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);	// anh tim contour
	Mat grayImg;    // anh xam
	cvtColor(src, grayImg, COLOR_RGB2GRAY);

	Mat cloneImg; //= cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
	cloneImg = src.clone();

	// tien xu ly anh
	//cvCopy(grayImg, contourImg);
	grayImg.copyTo(contourImg);
	//normalize(contourImg, contourImg, 0, 255, NORM_MINMAX);
	//imshow("contourImg", contourImg);
	//contourImg = ImageRestoration(contourImg).clone();

	threshold(contourImg, contourImg, 180, 255, THRESH_BINARY);
	//Mat element = getStructuringElement(MORPH_RECT, cv::Size(3, 3));
	//dilate(contourImg, contourImg, element, Point(-1, -1), 2);
	//Mat or_binary = binary.clone();
	//erode(contourImg, contourImg, element, Point(-1, -1), 2);
	imshow("contourImg", contourImg);
	
	//cout << "ha ha" << '\n';
	Mat rectImg; //= cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
	src.copyTo(rectImg);
	//cvMerge(contourImg, contourImg, contourImg, NULL, rectImg); // tron anh

	// tim contour cua buc anh

	vector<vector<cv::Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(contourImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	//cvShowImage("contourImg", contourImg);

	int w, h, s, ratio_plate_src;
	int count;
	double ratio_plate;	// ty le chieu rong tren chieu cao
	Rect rectPlate;



	//cout << contours.size() << '\n';

	for (size_t i = 0; i < contours.size(); i++)
	{
		/*count = contours->total;
		CvPoint *PointArray = new CvPoint[count];
		cvCvtSeqToArray(contours, PointArray, CV_WHOLE_SEQ);*/

		Rect r_Plate = boundingRect(contours.at(i));
		r_Plate = Rect(r_Plate.x, r_Plate.y + 2, r_Plate.width - 3, r_Plate.height - 2);
		rectangle(cloneImg, r_Plate, GREEN, 2);

		w = r_Plate.width;
		h = r_Plate.height;
		s = w * h;

		//cvRectangle(rectImg, cvPoint(xmin, ymin), cvPoint(xmax, ymax), RED);

		// loai bo nhung hinh chu nhat co ti le khong dung
		if (s != 0) {
			ratio_plate_src = (contourImg.size[0] * contourImg.size[1]) / s;
		}
		else {
			ratio_plate_src = 1000;
		}

		if (h == 0) {
			ratio_plate = 0;
		}
		else {
			ratio_plate = (double)w / h;
		}

		if (ratio_plate_src > 30 && ratio_plate_src < 270) {
			// ve ra hcn mau xanh la
			//cvRectangle(rectImg, cvPoint(xmin, ymin), cvPoint(xmax, ymax), GREEN);

			if (ratio_plate > 2.6 && ratio_plate < 7) {
				//cvRectangle(rectImg, cvPoint(xmin, ymin), cvPoint(xmax, ymax), BLUE);

				if (w > 80 && w < 250 && h > 25 && h < 150) {


					// cat bien so
					//plate = cvCreateImage(cvSize(rectPlate.width, rectPlate.height), IPL_DEPTH_8U, 3);

					plate = src(r_Plate);
					rectangle(rectImg, r_Plate, RED, 1);

					// luu vao mang cac bien so plateArr
					int cnt = CountCharacter(plate);
					if (cnt >= 5) {

						plates.push_back(plate);


					}
				}
			}
		}


	}

	imshow("cloneImg", cloneImg);
	imshow("rectImage", rectImg);
	//waitKey(0);
	contourImg.release();
	cloneImg.release();
	rectImg.release();
	grayImg.release();
	//element.release();

	return plates;

}


vector<Mat> PlateFinder::FindPlate_Car(Mat src) {
	//car_plate = 1;
	Mat plate;
	vector<Mat> plates;
	Mat contourImg; //= cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);	// anh tim contour
	Mat grayImg;    // anh xam
	cvtColor(src, grayImg, COLOR_RGB2GRAY);

	Mat cloneImg; //= cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
	cloneImg = src.clone();

	// tien xu ly anh
	//cvCopy(grayImg, contourImg);
	grayImg.copyTo(contourImg);
	normalize(contourImg, contourImg, 0, 255, NORM_MINMAX);
	//imshow("contourImg", contourImg);
	ImageRestoration_Car(contourImg);
	imshow("contourImg", contourImg);
	//waitKey(0);
	Mat rectImg; //= cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
	src.copyTo(rectImg);
	//cvMerge(contourImg, contourImg, contourImg, NULL, rectImg); // tron anh

	// tim contour cua buc anh

	vector<vector<cv::Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(contourImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	//cvShowImage("contourImg", contourImg);

	int xmin, ymin, xmax, ymax, w, h, s, ratio_plate_src;
	int count;
	double ratio_plate;	// ty le chieu rong tren chieu cao
	Rect rectPlate;


	//cout << contours.size() << '\n';

	for (size_t i = 0; i < contours.size(); i++)
	{
		/*count = contours->total;
		CvPoint *PointArray = new CvPoint[count];
		cvCvtSeqToArray(contours, PointArray, CV_WHOLE_SEQ);*/

		Rect r_Plate = boundingRect(contours.at(i));
		/*if (r_Plate.y >= 3)
			r_Plate = Rect(r_Plate.x - 2, r_Plate.y - 3, r_Plate.width - 3, r_Plate.height + 2);
		else
			r_Plate = Rect(r_Plate.x - 2, r_Plate.y, r_Plate.width - 3, r_Plate.height);*/

		//cout << "test" << r_Plate.y << '\n';

		rectangle(cloneImg, r_Plate, GREEN, 2);

		w = r_Plate.width;
		h = r_Plate.height;
		s = w * h;

		//cvRectangle(rectImg, cvPoint(xmin, ymin), cvPoint(xmax, ymax), RED);

		// loai bo nhung hinh chu nhat co ti le khong dung
		if (s != 0) {
			ratio_plate_src = (contourImg.size[0] * contourImg.size[1]) / s;
		}
		else {
			ratio_plate_src = 1000;
		}

		if (h == 0) {
			ratio_plate = 0;
		}
		else {
			ratio_plate = (double)w / h;
		}

		if (ratio_plate_src > 30 && ratio_plate_src < 270) {
			// ve ra hcn mau xanh la
			//cvRectangle(rectImg, cvPoint(xmin, ymin), cvPoint(xmax, ymax), GREEN);

			if (ratio_plate > 2.6 && ratio_plate < 7) {
				//cvRectangle(rectImg, cvPoint(xmin, ymin), cvPoint(xmax, ymax), BLUE);

				if (w > 80 && w < 250 && h > 25 && h < 150) {


					// cat bien so
					//plate = cvCreateImage(cvSize(rectPlate.width, rectPlate.height), IPL_DEPTH_8U, 3);

					plate = src(r_Plate);
					rectangle(rectImg, r_Plate, RED, 1);

					// luu vao mang cac bien so plateArr
					int cnt = CountCharacter(plate);
					if (cnt >= 5) {

						plates.push_back(plate);


					}
				}
			}
		}


	}



	//}
	imshow("cloneImg", cloneImg);
	imshow("rectImage", rectImg);
	contourImg.release();
	cloneImg.release();
	rectImg.release();
	grayImg.release();

	//if (plates.size() != 0)
	return plates;
	//else
	//	return cv::Mat(55, 65, CV_8UC3, GREEN);
}


vector<Mat> PlateFinder::FindPlate_Cascade_Car(Mat src) {
	// Detect LP
	vector<Mat> plates;
	vector<Rect> Licence_plate;
	vector<Rect>::iterator abc = Licence_plate.begin();
	Mat src_clone = src.clone();
	// Load LP Localization cascade (.xml file)
	CascadeClassifier LPL_cascade;
	//LPL_cascade.load("output-hv-33-x25.xml");
	//LPL_cascade.load("vn2.xml");
	LPL_cascade.load("au.xml");
	LPL_cascade.detectMultiScale(src, Licence_plate, 1.1, 3, 0 | CV_HAAR_FIND_BIGGEST_OBJECT, Size(10, 10));

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


int PlateFinder::CountCharacter(Mat plate) {
	int cnt = 0;
	Mat resizeImg, binaryImg;
	imshow("nam", plate);
	int w = plate.size[1];
	int h = plate.size[0];
	//resizeImg = Mat(Size(130, 110), plate.depth, 3);
	//binaryImg = Mat(Size(130, 110), plate.depth, 1);

	if (car_plate == 0)
		resize(plate, resizeImg, Size(w * 2, h * 2));
	else 
		resize(plate, resizeImg, Size(408,70));
	cout << w << " " << h << '\n';
	imshow("resizeImg1", resizeImg);
	//waitKey(0);
	cvtColor(resizeImg, binaryImg, COLOR_RGB2GRAY);
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
	bitwise_not(binaryImg, binaryImg);
	imshow("binaryImg", binaryImg);

	

	//cvShowImage("binaryImg", binaryImg);

	vector<vector<cv::Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(binaryImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	


	/*Phien ban tim ky tu tho-------------------------------------------------*/
	//for (size_t i = 0; i < contours.size(); i++)
	//{
	//	Rect rect = boundingRect(contours.at(i));
	//	rectangle(resizeImg, Point(rect.x, rect.y),
	//		Point(rect.x + rect.width, rect.y + rect.height), RED, 2);
	//	//cout << rect.width << " " << rect.height << '\n';
	//	if (rect.width > (w*2.0*0.045) && rect.width < (w*2.0/4)
	//		&& rect.height > (h*2.0*0.3) && rect.height < (h*2.0*0.6)
	//		&& rect.width * rect.height > (w*4.0*h*0.015))
	//	{
	//		rectangle(resizeImg, Point(rect.x, rect.y),
	//			Point(rect.x + rect.width, rect.y + rect.height), GREEN, 2);
	//		cnt++;
	//	}
	//} 
	/*Phien ban tim ky tu tho-------------------------------------------------*/


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
			rectangle(resizeImg, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), RED, 2);
			//cout << rect.width << "x" << rect.height << '\n';
			
			ratio = (double)rect.width / rect.height;
			s = rect.width * rect.height;
			//cout << s << '\n';

			//cvSetImageROI(binaryImg, r);
			white = countNonZero(binaryImg(rect));
			//cvResetImageROI(binaryImg);
			ratioWhite = (double)white / s;
			//cout << ratioWhite << '\n';

			if (ratio > ratio_min && ratio < ratio_max &&
				s > smin && rect.width > w_min && rect.width < w_max &&
				rect.height > h_min && rect.height < h_max &&
				ratioWhite > ratioWhite_min && ratioWhite < ratioWhite_max) //&& rect.x > 2)
			{
				rectangle(resizeImg, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), GREEN, 2);

				cnt++;
			}

			//contours = contours->h_next;
		}
	}

	//cvShowImage("resizeImg", resizeImg);
	imshow("resizeImg", resizeImg);
	//waitKey(0);
	resizeImg.release();
	binaryImg.release();
	element.release();

	return cnt;
}