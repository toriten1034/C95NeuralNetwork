#include "activate.hpp"
#include <iostream>
int main(void)
{
        mat::Matrix<Fix18> t(10, 1);
        t << 0 << 0 << 1 << 0 << 0 << 0 << 0 << 0 << 0 << 0;

        mat::Matrix<Fix18> y1(10, 1);
        y1 << 0.1 << 0.05 << 0.6 << 0.0 << 0.05 << 0.1 << 0.0 << 0.1 << 0.0 << 0.0;
        std::cout << cross_entropy_error(t, y1) << std::endl;

        mat::Matrix<Fix18> y2(10, 1);
        y2 << 0.1 << 0.05 << 0.1 << 0.0 << 0.05 << 0.1 << 0.0 << 0.6 << 0.0 << 0.0;
        std::cout << cross_entropy_error(t, y2) << std::endl;
}
