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
Fix18 sigmoid_gradient(Fix18 x)
{
	double tmp = (1.0 / (std::exp(-x.to_double()) + 1.0));
	double grad = (1.0 - tmp) * tmp;
	return Fix18(grad);
}

Fix18 relu(Fix18 x)
{
	return (0 < x.to_double()) ? x.to_double() : Fix18(0);
}

Fix18 relu_gradient(Fix18 x)
{
	return (0 < x.to_double()) ? 1 : 0;
}

mat::Matrix<Fix18> softmax(mat::Matrix<Fix18> obj)
{
	assert(obj.width * obj.height * obj.channel > 1);
	mat::Matrix<double> obj_double(obj.width, obj.height, obj.channel);
	//copy to double
	for (int i = 0; i < obj.width * obj.height * obj.channel; i++)
	{
		obj_double.data[i] = obj.data[i].to_double();
	}

	if (obj.height == 1 && obj.channel == 1)
	//1D soft max
	{
		mat::Matrix<double> max = obj_double.max(0);
		mat::Matrix<double> off_set = obj_double - max;
		mat::Matrix<double> off_exp = off_set.apply(std::exp);
		mat::Matrix<double> off_exp_sum = off_exp.sum(0);
		mat::Matrix<double> soft_max = off_exp / off_exp_sum;
	}
	else if (obj.height != 1)
	//2D soft max
	{

		mat::Matrix<double> max = obj_double.max(0);
		mat::Matrix<double> off_set = obj_double - max;
		mat::Matrix<double> off_exp = off_set.apply(std::exp);
		mat::Matrix<double> off_exp_sum = off_exp.sum(0);
		mat::Matrix<double> soft_max = off_exp / off_exp_sum;

		mat::Matrix<Fix18> soft_max_fix18(soft_max.width, soft_max.height, soft_max.channel);
		for (int copy_i = 0; copy_i < (soft_max.width * soft_max.height * soft_max.channel); copy_i++)
		{
			soft_max_fix18.data[copy_i] = soft_max.data[copy_i];
		}
		return soft_max_fix18;
	}
	else
	{
		std::cout << "error" << std::endl;
	}
	//
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