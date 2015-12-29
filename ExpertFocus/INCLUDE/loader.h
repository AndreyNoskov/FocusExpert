#pragma once

#include <string>

class Options
{

public:

	Options(std::string);
	~Options();

	void Load(std::string path_);

	// Getters
	int			GetNumberOfImages();
	int			GetTotalNumberOfImages();
	int			GetCellSize();
	std::string GetPathToBase();
	std::string GetPathToStorage();

	// Setters

private:

	// main vars
	std::string path;
	// options vars
	int			number_of_images;
	int			total_number_of_images;
	int			cell_size;
	std::string path_to_base;
	std::string path_to_storage;

};