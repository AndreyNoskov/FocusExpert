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

	cell_size = GetPrivateProfileIntA(
		LPCTSTR("base"),
		LPCTSTR("cell_size"),
		50,
		LPCTSTR(path.c_str()));

	total_number_of_images = GetPrivateProfileIntA(
		LPCTSTR("base"),
		LPCTSTR("total_number_of_images"),
		37,
		LPCTSTR(path.c_str()));

	char str_base[255];
	GetPrivateProfileStringA(
		LPCTSTR("base"),
		LPCTSTR("path_to_base"),
		"../ExpertFocus/RESOURCE/base/",
		str_base,
		sizeof(str_base),
		LPCTSTR(path.c_str()));
	path_to_base.assign(str_base);

	char str_storage[255];
	GetPrivateProfileStringA(
		LPCTSTR("base"),
		LPCTSTR("path_to_storage"),
		"../ExpertFocus/RESOURCE/storage/",
		str_storage,
		sizeof(str_storage),
		LPCTSTR(path.c_str()));
	path_to_storage.assign(str_storage);
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

std::string Options::GetPathToStorage()
{
	return path_to_storage;
}

int Options::GetCellSize()
{
	return cell_size;
}