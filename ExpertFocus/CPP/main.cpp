#include <opencv2/opencv.hpp>
#include <iostream>

#include <msc.h>
#include <options.h>

int main()
{
	std::cout << CV_VERSION << std::endl;
	std::list<std::string> imageList = GetImageSet();
	std::list<std::string>::iterator it = imageList.begin();
	
	for (it = imageList.begin(); it != imageList.end(); ++it)
	{
		cv::Mat img1 = cv::imread(*it);
		//cv::Mat img2 = cv::imread(*it);
		std::vector<cv::Point2i> tmp1 =  GetCenters_optimize(img1.cols, img1.rows, 50.0f);
		//std::vector<cv::Point2i> tmp2  = GetCenters(img2.cols, img2.rows, 200.0f);
		cv::imshow("Image_opt", DrawCenters(img1, tmp1, 50));
		//cv::imshow("Image_norm", DrawCenters(img2, tmp2));
		cv::waitKey();
	}
	cv::destroyAllWindows();
	system("pause");
	return 0;
}