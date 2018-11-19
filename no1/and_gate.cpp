#include <stdio.h>
#include <iostream>
#include <vector>
#include <numeric>
#include "../fixed_19/fixed_point.hpp"

Fix18 AND(Fix18 x1, Fix18 x2)
{

	std::vector<Fix18> w = {0.5, 0.5};

	Fix18 b(-0.7);
	Fix18 tmp = x1 * w[0] + x2 * w[1] + b;
	if (tmp.to_double() <= 0)
	{
		return Fix18(0.0);
	}
	else
	{
		return Fix18(1.0);
	}
}

Fix18 NAND(Fix18 x1, Fix18 x2)
{

	std::vector<Fix18> w = {-0.5, -0.5};
	Fix18 b(0.7);
	Fix18 tmp = x1 * w[0] + x2 * w[1] + b;
	if (tmp.to_double() <= 0)
	{
		return Fix18(0.0);
	}
	else
	{
		return Fix18(1.0);
	}
}

Fix18 OR(Fix18 x1, Fix18 x2)
{

	std::vector<Fix18> w = {0.5, 0.5};

	Fix18 b(-0.2);
	Fix18 tmp = x1 * w[0] + x2 * w[1] + b;
	if (tmp.to_double() <= 0)
	{
		return Fix18(0.0);
	}
	else
	{
		return Fix18(1.0);
	}
}

Fix18 XOR(Fix18 x1, Fix18 x2)
{

	Fix18 s1 = NAND(x1, x2);
	Fix18 s2 = OR(x1, x2);
	Fix18 y = AND(s1, s2);
	return y;
}
