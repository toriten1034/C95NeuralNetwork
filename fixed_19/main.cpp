#include <iostream>
#include <assert.h>
#include "fixed_point.hpp"

int main(void)
{

	Fix18 a = 262138;
	std::cout << a << std::endl;

	Fix18 b = 269;
	std::cout << b << std::endl;

	Fix18 c = a * b;
	std::cout << "difference" << c << std::endl;

	/*
	for (double i = -20.0; i < 20.0; i += 0.1)
	{
		for (double j = -1.0; j < 1.0; j += 0.01)
		{

			Fix18 a = i;
			Fix18 b = j;
			double c = (a * b).to_double() - (i * j);
			std::cout << "difference" << c << std::endl;
			assert(c < 1.0 && c > -1.0);
		}
	}
	*/
}