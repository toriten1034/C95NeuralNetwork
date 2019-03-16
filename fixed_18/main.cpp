#include <iostream>
#include <assert.h>
#include <string>
#include "fixed_point.hpp"

std::string int2bin(int x)
{
	int mask = 0x20000;
	std::string bits;
	for (int i = 0; i < 18; i++)
	{
		bits += (mask & x) ? "1" : "0";
		mask >>= 1;
	}
	return bits;
}

int main(void)
{

	Fix18 max;
	max = -1.0;
	std::cout << "bit field   max :" << int2bin(max.value) << std::endl;
	std::cout << "max value " << max << std::endl;
}