#pragma once

#include <opencv2/opencv.hpp>
#include <string>

class Session
{

public:
	Session::Session(std::string path_storage, std::string path_img);
	~Session();
	
	cv::Mat* GetMat() { return &tmpMat; }

private:
	cv::FileStorage fs;
	std::string path;
	cv::Mat tmpMat;
};