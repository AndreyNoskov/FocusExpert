#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

#include <msc.h>
#include <callbacks.h>
#include <loader.h>
#include <session.h>

int main()
{
	Options options("../ExpertFocus/RESOURCE/options.ini");
	std::string winName = "Focus Expert";

	char globalControl = 0;

	while (globalControl != 'q')
	{
		std::cout << "New session was started!\n";

		std::set<int> imageList = GetImageSet(&options);
		std::set<int>::iterator it = imageList.begin();

		cv::namedWindow(winName, 1);
		char sessionControl = 0;

		for (it = imageList.begin(); it != imageList.end(); ++it)
		{
			sessionControl = 0;

			std::string path_to_img = options.GetPathToBase() + std::to_string(*it) + ".jpg";
			std::string path_to_store = options.GetPathToStorage() + std::to_string(*it) + ".xml";

			Session session(path_to_store, path_to_img);

			cv::Mat img = cv::imread(path_to_img);
			cv::Mat mask(img.rows, img.cols, img.type(), cv::Scalar(0, 0, 0));
			cv::Mat blend(img.rows, img.cols, img.type(), cv::Scalar(0, 0, 0));

			std::vector<cv::Point2i> imgPoints = GetCenters_optimize(img.cols, img.rows, options.GetCellSize());
			std::vector<std::vector<cv::Point>> contours = CreateVoronoi(imgPoints, img.cols, img.rows);

			CallbackData cbd(&img, &mask, &blend, &winName, &contours);

			for (int i = 0; i < contours.size(); ++i)
				cv::drawContours(img, contours, i, cv::Scalar(0, 128, 255), 1);
			cv::addWeighted(img, 0.8, mask, 0.2, 0, blend);
			cv::imshow(winName, blend);

			cv::Mat* tmpMat = session.GetMat();
			
			cv::setMouseCallback(winName, on_mouse_click, &cbd);
			while (sessionControl != 'n')
			{
				sessionControl = cv::waitKey(0);
				if (sessionControl == 'n') {
					std::vector<cv::Mat> planes;
					cv::split(mask, planes);
					cv::threshold(planes[1], planes[1], 128, 1, CV_THRESH_BINARY);
					planes[1].convertTo(planes[1], CV_16UC1);
					cv::add(*tmpMat, planes[1], *tmpMat);
				}
			}
		}
		std::cout << "Your session is over!\n";
		std::cout << "Thank you for your participation\n";
		std::cout << "Press \"q\" for quit or any other key for continue\n\n";
		globalControl = cv::waitKey(0);
	}

	cv::destroyAllWindows();
	//system("pause");
	return 0;
}