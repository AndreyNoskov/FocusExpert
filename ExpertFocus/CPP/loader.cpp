#include <loader.h>

#include <windows.h>

Options::Options(std::string path)
{
	Load(path);
}

Options::~Options()
{

}

void Options::Load(std::string path_)
{
	path = path_;

	number_of_images = GetPrivateProfileIntA(
		LPCTSTR("base"),
		LPCTSTR("number_of_images"),
		5,
		LPCTSTR(path.c_str()));

	total_number_of_images = GetPrivateProfileIntA(
		LPCTSTR("base"),
		LPCTSTR("total_number_of_images"),
		37,
		LPCTSTR(path.c_str()));

	char str[255];
	GetPrivateProfileStringA(
		LPCTSTR("base"),
		LPCTSTR("path_to_base"),
		"../ExpertFocus/RESOURCE/",
		str,
		sizeof(str),
		LPCTSTR(path.c_str()));
	path_to_base.assign(str);

}

int Options::GetNumberOfImages()
{
	return number_of_images;
}

int Options::GetTotalNumberOfImages()
{
	return total_number_of_images;
}

std::string Options::GetPathToBase()
{
	return path_to_base;
}