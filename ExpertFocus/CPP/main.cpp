#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

#include <msc.h>
#include <options.h>
#include <callbacks.h>

int main()
{
	std::list<std::string> imageList = GetImageSet();
	std::list<std::string>::iterator it = imageList.begin();
	
	std::string winName = "Focus Expert";
	cv::namedWindow(winName, 1);
	char c = 0;

	for (it = imageList.begin(); it != imageList.end(); ++it)
	{
		c = 0;
		cv::Mat img = cv::imread(*it);
		cv::Mat mask(img.rows, img.cols, img.type(), cv::Scalar(0,0,0));
		cv::Mat blend(img.rows, img.cols, img.type(), cv::Scalar(0, 0, 0));
		std::vector<cv::Point2i> imgPoints =  GetCenters_optimize(img.cols, img.rows, 50.0f);
		std::vector<std::vector<cv::Point>> contours = CreateVoronoi(imgPoints, img.cols, img.rows);
		CallbackData cbd(&img, &mask, &blend, &winName, &contours);
		for (int i = 0; i < contours.size(); ++i)
			cv::drawContours(img, contours, i, cv::Scalar(255,0,0), 1);
		cv::addWeighted(img, 0.8, mask, 0.2, 0, blend);
		cv::imshow(winName, blend);
		cv::setMouseCallback(winName, on_mouse_click, &cbd);
		while (c != 'n') 
			c = cv::waitKey(0);
	}
	cv::destroyAllWindows();
	system("pause");
	return 0;
}