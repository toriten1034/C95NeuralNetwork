#include <iostream>
#include <map>
//
//#include "../fixed_19/fixed_point.hpp"
#include "../activation/activate.hpp"

int main(void)
{
	//initial
	mat::Matrix<Fix18> A(4, 2);
	A = A << 1.0 << 0.5 << -0.5 << -1.0
		  << -1.0 << 0.5 << -0.5 << 1.0;

	A.show();

	mat::Matrix<Fix18> B(4, 1);
	B = B << 0.0 << 0.0 << 1.0 << 0.0;
	B.show();

	mat::Matrix<Fix18> data1(2, 1);
	data1 = data1 << 0.0
				  << 1.0;
	data1.show();

	mat::Matrix<Fix18> C = data1.dot(A) + B;
	C.show();

	mat::Matrix<Fix18> data2(2, 1);
	data2 = data2 << 1.0
				  << 1.0;
	data2.show();

	C = data2.dot(A) + B;
	C.show();

	return 0;
}