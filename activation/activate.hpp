#include <numeric>
#include <assert.h>
#include "../fixed_19/fixed_point.hpp"
#include "../matrix/matrix.hpp"
#include <stdlib.h>

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
	return Fix18(tmp);
}

Fix18 relu(Fix18 x)
{
	return (0 < x.to_double()) ? x.to_double() : Fix18(0);
}

mat::Matrix<Fix18> softmax(mat::Matrix<Fix18> obj)
{
	int end;
	if (obj.height != 0)
	//multi dimension
	{
		end = obj.width * obj.height * obj.width;
	}
	else
	//1 dimension
	{
		end = obj.width;
	}
	mat::Matrix<Fix18> max = obj.max(0);

	mat::Matrix<Fix18> tmp = obj.clone();
	mat::Matrix<Fix18> result = obj.clone();

	for (int i = 0; i < end; i++)
	{
		tmp.data[i] = std::exp((obj.data[i] - max.data[i]).to_double());
	}
	double sum = tmp.sum(0).data[0].to_double();
	for (int i = 0; i < end; i++)
	{
		result.data[i] = Fix18(result.data[i].to_double() / sum);
	}
}

double cross_entropy_error(mat::Matrix<Fix18> y, mat::Matrix<Fix18> t)
{
	assert(y.width == t.width && y.height == t.height && y.channel == t.channel);

	double delta = 1.0 / std::pow(10, 7);
	mat::Matrix<double> tmp_y(y.width, y.height, y.channel);
	for (int i = 0; i < (y.width * y.height * y.channel); i++)
	{
		tmp_y.data[i] = y.data[i].to_double();
	}

	mat::Matrix<double> tmp_t(t.width, t.height, t.channel);
	for (int i = 0; i < (t.width * t.height * t.channel); i++)
	{
		tmp_t.data[i] = t.data[i].to_double();
	}

	mat::Matrix<double> result(t.width, t.height, t.channel);
	for (int i = 0; i < (y.width * y.height * y.channel); i++)
	{
		result.data[i] = tmp_y.data[i] * std::log(tmp_t.data[i] + delta);
	}
	double sum = -result.sum(0).data[0];
}