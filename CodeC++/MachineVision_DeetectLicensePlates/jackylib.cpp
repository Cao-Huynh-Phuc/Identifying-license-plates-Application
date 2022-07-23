#include "jackylib.h"
using namespace jacky_lib;
str2char::str2char()
{	
}


mat2picture::mat2picture()
{
}
System::Drawing::Bitmap^ mat2picture::Mat2Bimap(cv::Mat & colorImage) {
	//System::Drawing::Graphics^ graphics = control->CreateGraphics();
	System::IntPtr ptr(colorImage.ptr());
	System::Drawing::Bitmap^ b;
	switch (colorImage.type())
	{
	case CV_8UC3: // non-grayscale images are correctly displayed here
		b = gcnew System::Drawing::Bitmap(colorImage.cols, colorImage.rows, colorImage.step,
			System::Drawing::Imaging::PixelFormat::Format24bppRgb, ptr);
		break;
	case CV_8UC1: // grayscale images are incorrectly displayed here 
		b = gcnew System::Drawing::Bitmap(colorImage.cols, colorImage.rows, colorImage.step,
			System::Drawing::Imaging::PixelFormat::Format8bppIndexed, ptr);
		break;
	default:
		// error message
		break;
	}
	return b;
}
