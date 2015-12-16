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
		cv::Mat img = cv::imread(*it);
		std::vector<cv::Point2i> tmp = GetCenters_optimize(img.rows, img.cols, 200.0f);
		cv::imshow("Image", DrawCenters(img, 200.0f));
		cv::waitKey(3000);
	}
	cv::destroyAllWindows();
	system("pause");
	return 0;
}