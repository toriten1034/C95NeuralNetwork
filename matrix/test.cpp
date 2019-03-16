#include <iostream>
#include <map>
#include "activate.hpp"
//
//#include "../fixed_19/fixed_point.hpp"
//#include "../activation/activate.hpp"

int main(void)
{
	//initial
	mat::Matrix<double> W1(4, 2);
	W1 = W1 << -0.355949 << -2.08092 << -2.25596 << -3.00595
			<< 0.0156546 << -2.36924 << 1.16589 << -0.205226;

	mat::Matrix<double> b1(4, 1);
	b1 = b1 << -0.332347 << 1.49746 << 0.0589297 << 0.865592;

	mat::Matrix<double> W2(4, 4);
	W2 = W2 << 0.0567501 << 0.0715665 << 0.0486049 << -0.154805
			<< 0.112365 << -2.50768 << -0.279394 << 2.69622
			<< -1.50046 << -0.437279 << 1.70032 << 0.263788
			<< -1.31754 << -1.71381 << 1.36947 << 1.66626;

	mat::Matrix<double>
		b2(4, 1);
	b2 = b2 << 1.14017 << 1.4613 << -0.898386 << -1.70308;

	//calclate
	mat::Matrix<double> x0(2, 1);
	x0 = x0 << 1.0 << 0.0;
	W1.show();
	x0.show();

	//calclate
	mat::Matrix<double> x1(2, 1);
	x1 = x1 << 1.0 << 1.0;

	//calclate
	mat::Matrix<double> x2(2, 1);
	x2 = x2 << 0.0 << 1.0;

	//calclate
	mat::Matrix<double> x3(2, 1);
	x3 = x3 << 0.0 << 0.0;

	mat::Matrix<double> a1 = x0.dot(W1) + b1;
	mat::Matrix<double> z1 = a1.apply(sigmoid);
	mat::Matrix<double> a2 = z1.dot(W2) + b2;
	a2.show();

	a1 = x1.dot(W1) + b1;
	z1 = a1.apply(sigmoid);
	a2 = z1.dot(W2) + b2;
	a2.show();

	a1 = x2.dot(W1) + b1;
	z1 = a1.apply(sigmoid);
	a2 = z1.dot(W2) + b2;
	a2.show();

	a1 = x3.dot(W1) + b1;
	z1 = a1.apply(sigmoid);
	a2 = z1.dot(W2) + b2;
	a2.show();

	return 0;
}