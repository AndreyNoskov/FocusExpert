#include <msc.h>

std::list<std::string> GetImageSet()
{
	std::list<std::string> imageList;
	while (imageList.size() < NUMBER_OF_IMAGES)
	{
		size_t imgNum = rand() % TOTAL_NUMBER_OF_IMAGES;
		std::string path = "../RESOURCE/ExpertFocus/";
		std::string name(std::to_string(imgNum) + ".jpg");
		path.append(name);
		imageList.push_back(path);
	}
	return imageList;
}