#pragma  once

// std includes
#include <set>
#include <list>
#include <string>
#include <vector>

// opencv includes
#include <opencv2/opencv.hpp>

// local includes
#include <loader.h>

std::set<int> GetImageSet(Options* options);

std::vector<cv::Point2i> GetCenters(size_t width, size_t height, int minDist);
std::vector<cv::Point2i> GetCenters_optimize(size_t width, size_t height, int minDist);

cv::Mat DrawCenters(cv::Mat& src, std::vector<cv::Point2i>& centers, int radius);

std::vector<std::vector<cv::Point>> CreateVoronoi(std::vector<cv::Point2i>& points, int width, int height);

