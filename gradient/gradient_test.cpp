#include <iostream>
#include <cmath>
#include "../matrix/matrix.hpp"
#include "../gradient/gradient.hpp"

double function_2(mat::Matrix<double> x)
{
        return (x.data[0] * x.data[0]) + (x.data[1] * x.data[1]);
}
int main(void)
{
        /*
        mat::Matrix<Fix18> A(2, 1), B(2, 1), C(2, 1);
        A << 3.0 << 4.0;
        B << 0.0 << 2.0;
        C << 3.0 << 0.0;
        grad::numerical_gradient(function_2, A).show();
        grad::numerical_gradient(function_2, B).show();
        grad::numerical_gradient(function_2, C).show();
        */

        mat::Matrix<Fix18> X(3, 3);
        X.shape();
        X = X.random();
        //  X.show();
        mat::Matrix<Fix18> X0 = X.max(0);
        mat::Matrix<Fix18> X1 = X.max(1);
        X.show();
        X0.show();
        X1.show();
}
