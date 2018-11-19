#include <iostream>
#include <map>
//
//#include "../fixed_19/fixed_point.hpp"
#include "../activation/activate.hpp"

int main(void)
{
	//initial
	mat::Matrix<Fix18> W1(3, 2);
	W1 << 0.1 << 0.3 << 0.5
	   << 0.2 << 0.4 << 0.6;

	W1.show();

	mat::Matrix<Fix18> b1(3, 1);
	b1 << 0.1 << 0.2 << 0.3;

	mat::Matrix<Fix18> W2(2, 3);
	W2 << 0.1 << 0.4
	   << 0.2 << 0.5
	   << 0.3 << 0.6;

	mat::Matrix<Fix18> b2(2, 1);
	b2 << 0.1 << 0.2;

	mat::Matrix<Fix18> W3(2, 2);
	W3 << 0.1 << 0.3
	   << 0.2 << 0.4;

	mat::Matrix<Fix18> b3(2, 1);
	b3 << 0.1 << 0.2;

	//calclate
	mat::Matrix<Fix18> x(2, 1);
	x << 1.0 << 0.5;

	mat::Matrix<Fix18> a1 = x.dot(W1) + b1;
	mat::Matrix<Fix18> z1 = a1.apply(sigmoid);

	mat::Matrix<Fix18> a2 = z1.dot(W2) + b2;
	mat::Matrix<Fix18> z2 = a2.apply(sigmoid);

	mat::Matrix<Fix18> a3 = z2.dot(W3) + b3;
	a3.shape();
	a3.show();

	mat::Matrix<Fix18> soft = softmax(a3);
	soft.show();
	return 0;
}