#include <iostream>
#include <fstream>
#include <cstdlib>

#include "png.h"

void echo_usage()
{
	std::cerr << "Usage: alpha-check <input> [-n]" << std::endl;
	std::cerr << "[-n]: just test" << std::endl;
}

int main(int argc, char *argv[])
{
	if (argc < 2 || argc > 3)
	{
		echo_usage();
		return 1;
	}


	const char *input = argv[1];
	bool strip = true;
	if (argc == 3)
	{
		if (strcmp(argv[2], "-n") != 0)
		{
			echo_usage();
			return 1;
		}
		else
		{
			strip = false;
		}
	}

	int w, h, c;

	unsigned char *data = png_load(input, &w, &h, &c);

	if (data == 0)
	{
		std::cerr << "Error loading image. Must be PNG format." << std::endl;
		return 1;
	}

	if (c != 4)
	{
		delete[] data;
		return 0;
	}

	const size_t N = 4 * w * h;
	size_t emptyN = 0;
	size_t fullN = 0;
	for (size_t i = 3; i < N; i += 4)
	{
		if (data[i] == 0)
		{
			emptyN++;
		}
		else if (data[i] > 250)
		{
			fullN++;
		}
	}

	if (strip)
	{
		if (fullN == w * h)
		{
			png_save24(input, w, h, data);
			std::cout << input << "converted!" << std::endl;
		}
	}
	else
	{
		std::cout << input << ", " << c << ", " << emptyN << ", " << fullN << ", " << w * h << std::endl;
	}

	delete[] data;

	return 0;
}
