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
	std::string GetPathToBase();

	// Setters

private:

	// main vars
	std::string path;
	// options vars
	int			number_of_images;
	int			total_number_of_images;
	std::string path_to_base;
};