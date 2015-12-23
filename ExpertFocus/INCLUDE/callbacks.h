#pragma once

#include <string>

#include <opencv2/opencv.hpp>

struct CallbackData {
	CallbackData(cv::Mat* img,
				 cv::Mat* mask,
				 cv::Mat* blend,
				 std::string* winName,
				 std::vector<std::vector<cv::Point>>* contours) :
		img(img), mask(mask), blend(blend), winName(winName), contours(contours) {}
	cv::Mat* blend;
	cv::Mat* img;
	cv::Mat* mask;
	std::string* winName;
	std::vector<std::vector<cv::Point>>* contours;
};

void on_mouse_click(int e, int x, int y, int d, void*  ptr);