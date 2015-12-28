#include "callbacks.h"

void on_mouse_click(int e, int x, int y, int d, void*  ptr)
{
	if (e == CV_EVENT_LBUTTONDOWN)
	{
		CallbackData* cbd = (CallbackData*)ptr;
		std::vector<std::vector<cv::Point>>* contours = cbd->contours;
		for (int i = 0; i < contours->size(); ++i)
			if (cv::pointPolygonTest((*(contours))[i], cv::Point(x, y), false) >= 0)
			{
				if ( cbd->mask->at<cv::Vec3b>(cv::Point(x, y))[1] == 0 ) 
					cv::fillConvexPoly(*(cbd->mask), (*(contours))[i], cv::Scalar(0, 255, 0));
				else
					cv::fillConvexPoly(*(cbd->mask), (*(contours))[i], cv::Scalar(0, 0, 0));
				break;
			}
		double weight = 0.8;
		cv::addWeighted(*(cbd->img), weight, *(cbd->mask), 1 - weight, 0.0, *(cbd->blend));
		cv::imshow(*(cbd->winName), *(cbd->blend));
		//cv::updateWindow(*(cbd->winName));
	}
}