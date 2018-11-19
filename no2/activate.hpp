#include <numeric>
#include <assert.h>
#include <cmath>
#include "../fixed_19/fixed_point.hpp"

Fix18 step_function(Fix18 x)
{
	if (0 < x.to_double())
	{
		return Fix18(1.0);
	}
	else
	{
		return Fix18(0.0);
	}
}

Fix18 sigmoid(Fix18 x)
{

	double tmp = 1.0 / (std::exp(-x.to_double()) + 1.0);
	std::cout << tmp << std::endl;
	return Fix18(tmp);
	/*
	assert("まだ出来てないの");
	return EXIT_SUCCESS;*/
}

Fix18 relu(Fix18 x)
{
	return (0 < x.to_double()) ? x.to_double() : Fix18(0);
}
