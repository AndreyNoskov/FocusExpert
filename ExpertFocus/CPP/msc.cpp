#include <msc.h>
#include <chrono>
#include <random>

struct TmpPair
{
	TmpPair() : prev(nullptr), next(nullptr), x(0), y(0), marked(false) {}
	int x;
	int y;
	TmpPair* prev;
	TmpPair* next;
	bool marked;
};

const TmpPair* GetElement(const TmpPair* arr, int index) {
	const TmpPair* next = arr;
	int counter = 0;
	while (counter < index) {
		if (next->next == nullptr)
			return nullptr;
		next = next->next;
		counter++;
	}
	return next;
}

std::list<std::string> GetImageSet()
{
	std::list<std::string> imageList;
	std::default_random_engine re(time(0));

	while (imageList.size() < NUMBER_OF_IMAGES)
	{
		std::uniform_real_distribution<double> unif(0, TOTAL_NUMBER_OF_IMAGES);
		int imgNum = (int)unif(re);
		std::string path = "../ExpertFocus/RESOURCE/";
		std::string name(std::to_string(imgNum) + ".jpg");
		path.append(name);
		imageList.push_back(path);
	}
	return imageList;
}

std::vector<cv::Point2i> GetCenters(size_t width, size_t height, int minDist)
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
	for (int i = 0; i < width * height; ++i)
	{
		allPoints[i].prev = allPoints + i - 1;
		allPoints[i].next = allPoints + i + 1;
		allPoints[i].x	  = i % width;
		allPoints[i].y    = i / width;
	}
	
	allPoints[0].prev = nullptr;
	allPoints[width * height - 1].next = nullptr;

	int listLength = width * height;
	std::default_random_engine re(time(0));

	int sqrDist = minDist * minDist;

	cv::Mat img(height, width, CV_8UC3);
	img.zeros(height, width, CV_8UC3);

	while (listLength > 0)
	{
		std::uniform_real_distribution<double> unif(0, listLength);
		int toAdd = (int)unif(re);

		const TmpPair* tmpPoint = GetElement(allPoints, toAdd);
		if (tmpPoint == nullptr)
		{
			std::cout << "NullPtr was returned with listLength = " << listLength
				      << " and index " << toAdd << "\n";
			return outs;
		}
		cv::Point2i point(tmpPoint->x, tmpPoint->y);
		outs.push_back(point);

		int lft = (point.x - minDist < 0 )		   ? 0		    : point.x - minDist;
		int rgt = (point.x + minDist > width - 1)  ? width - 1  : point.x + minDist;
		int top = (point.y - minDist < 0 )		   ? 0		    : point.y - minDist;
		int bot = (point.y + minDist > height - 1) ? height - 1 : point.y + minDist;

		for (int i = lft; i <= rgt; ++i)
			for (int j = top; j <= bot; ++j)
			{
				int index = j * width + i;
				if (!allPoints[index].marked)
				{
					double dist = (point.x - allPoints[index].x)*(point.x - allPoints[index].x) + 
								  (point.y - allPoints[index].y)*(point.y - allPoints[index].y);
					if (dist < sqrDist)
					{
						if (allPoints[index].prev != nullptr)
							allPoints[index].prev->next = allPoints[index].next;
						if (allPoints[index].next != nullptr)
							allPoints[index].next->prev = allPoints[index].prev;
						allPoints[index].next = nullptr;
						allPoints[index].prev = nullptr;
						allPoints[index].marked = true;
						listLength--;
						cv::circle(img, cv::Point2i(allPoints[index].x, allPoints[index].y), 0, cv::Scalar(255, 0, 0));
					}
				}
			}

		cv::circle(img, cv::Point2i(point.x, point.y), 50, cv::Scalar(0, 0, 255));
		cv::imshow("Test", img);
		cv::waitKey(1);
		cv::Mat marked(height, width, CV_8UC1);
		marked.zeros(height, width, CV_8UC1);
		for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			cv::circle(marked, cv::Point(allPoints[].x, allPoints[].y), 0, cv::Scalar(0, 255, 255));
		cv::imshow("Test", img);
		cv::waitKey(1);
	}

	delete [] allPoints;

	std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
	double time = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() / 1000.0;
	std::cout << "\"GetCenters_opt\" function was executed in " << time << " seconds\n";
	std::cout << "Size: " << listLength << "\n";
	return outs;
}

cv::Mat DrawCenters(cv::Mat& src, std::vector<cv::Point2i>& centers, int radius)
{
	size_t width  = src.cols;
	size_t height = src.rows;
	cv::Mat out(src);
	for (size_t i = 0; i < centers.size(); ++i)
		cv::circle(out, centers[i], radius, cv::Scalar(0, 0, 255), 1);
	return out;
}