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
		cv::imshow("Image", img);
		cv::waitKey(3000);
	}
		

	system("pause");
	return 0;
}