#include <session.h>

Session::Session(std::string path_storage, std::string path_img)
{
	
	path = path_storage;

	if ( FILE *file = fopen(path.c_str(), "r" ))
		fclose(file);
	else {
		fs.open(path, cv::FileStorage::WRITE);
		cv::Mat tmp = cv::imread(path_img);
		tmpMat = cv::Mat::zeros(tmp.rows, tmp.cols, CV_16UC1);
		fs << "Mat" << tmpMat;
		fs.release();
	}

	if ( fs.open(path, cv::FileStorage::READ) )
	{
		 fs[ "Mat" ] >> tmpMat;
	}
	else
	{
		std::cout << "Can't open file " << path << " to read\n";
	}
}

Session::~Session()
{
	fs.release();
	fs.open(path, cv::FileStorage::WRITE);
	fs << "Mat" << tmpMat;
	fs.release();
}