#include <msc.h>
#include <chrono>
#include <random>

struct TmpPair
{
	TmpPair() : prev(nullptr), next(nullptr), x(0), y(0) {}
	int x;
	int y;
	TmpPair* prev;
	TmpPair* next;
};

TmpPair* GetElement(TmpPair* arr, int index) {
	TmpPair* next = arr;
	if (index == 0)
		return arr;
	for (int i = 1; i < index; ++i)
		if (next->next != nullptr)
			next = next->next;
		else return nullptr;
	return next;
}

std::list<std::string> GetImageSet()
{
	std::list<std::string> imageList;
	while (imageList.size() < NUMBER_OF_IMAGES)
	{
		size_t imgNum = rand() % TOTAL_NUMBER_OF_IMAGES;
		std::string path = "../ExpertFocus/RESOURCE/";
		std::string name(std::to_string(imgNum) + ".jpg");
		path.append(name);
		imageList.push_back(path);
	}
	return imageList;
}

std::vector<cv::Point2i> GetCenters(size_t width, size_t height, double minDist)
{
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

	std::vector<cv::Point2i> outs;
	std::list<cv::Point2i>   candidates;

	for (size_t i = 0; i < width; ++i)
		for (size_t j = 0; j < height; ++j)
			candidates.push_back(cv::Point2i(i, j));

	std::list<cv::Point2i>::iterator it = candidates.begin();
	std::default_random_engine re;

	while (candidates.size() > 0)
	{
		std::uniform_real_distribution<double> unif(0, candidates.size() - 1 );
		
		int toAdd = (int) unif(re);

		it = candidates.begin();
		std::advance(it, toAdd);
		cv::Point2i curPoint(*it);
		outs.push_back(curPoint);

		it = candidates.begin();
		while (it != candidates.end())
		{
			double dist = sqrt( ((*it).x - curPoint.x)*((*it).x - curPoint.x) +
								((*it).y - curPoint.y)*((*it).y - curPoint.y) );
			if (dist < minDist)
				it = candidates.erase(it);
			else
				++it;
		}
	}

	std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
	double time = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() / 1000.0;
	std::cout << "\"GetCenters\" function was executed in " << time << " seconds\n";
	return outs;
}

std::vector<cv::Point2i> GetCenters_optimize(size_t width, size_t height, int minDist)
{
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	std::vector<cv::Point2i> outs;
	
	TmpPair* allPoints = new TmpPair [width * height];
	for (size_t i = 0; i < width; ++i)
		for (size_t j = 0; j < height; ++j)
		{
			allPoints[i * width + j].prev = &allPoints[i*width + j - 1];
			allPoints[i * width + j].next = &allPoints[i*width + j + 1];
			allPoints[i * width + j].x = i;
			allPoints[i * width + j].y = j;
		}
	allPoints[0].prev = nullptr;
	allPoints[width * height - 1].next = nullptr;

	size_t listLength = width * height;

	std::default_random_engine re;
	minDist *= minDist;
	while (listLength > 0)
	{
		std::uniform_real_distribution<double> unif(0, listLength - 1);
		int toAdd = (int)unif(re);

		TmpPair* tmpPoint = GetElement(allPoints, toAdd);
		cv::Point2i point(tmpPoint->x, tmpPoint->y);

		int top = (tmpPoint->x - minDist < (size_t)0 ) ? 0 : tmpPoint->x - minDist;
		int bot = (tmpPoint->x + minDist > width) ? width : tmpPoint->x + minDist;
		int lft = (tmpPoint->y - minDist < (size_t)0 ) ? 0 : tmpPoint->y - minDist;
		int rgt = (tmpPoint->y + minDist > height) ? height : tmpPoint->y + minDist;

		for (int i = lft; i < rgt; ++i)
			for (int j = top; j < bot; ++j)
			{
				int index = j * width + i;
				double dist = (point.x - allPoints[index].x)*(point.x - allPoints[index].x) - 
							  (point.y - allPoints[index].y)*(point.y - allPoints[index].y);
				if (dist < minDist)
				{
					if (allPoints[index].prev != nullptr)
						allPoints[index].prev->next = allPoints[index].next;
					if (allPoints[index].next != nullptr)
						allPoints[index].next->prev = allPoints[index].prev;
					allPoints[index].next = nullptr;
					allPoints[index].prev = nullptr;
					listLength--;
				}
			}
	}

	delete [] allPoints;

	std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
	double time = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() / 1000.0;
	std::cout << "\"GetCenters_opt\" function was executed in " << time << " seconds\n";
	return outs;
}

cv::Mat DrawCenters(cv::Mat& src, double minDist)
{
	size_t width  = src.cols;
	size_t height = src.rows;
	std::vector<cv::Point2i> centers = GetCenters(width, height, minDist);
	cv::Mat out(src);
	for (size_t i = 0; i < centers.size(); ++i)
		cv::circle(out, centers[i], 3, cv::Scalar(0, 0, 255), -1);
	return out;
}