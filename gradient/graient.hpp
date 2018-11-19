#include "../matrix/matrix.hpp"
#include "../fixed_19/fixed_point.hpp"

namespace grad
{
mat::Matrix<Fix18> numerical_gradient(double (*func)(mat::Matrix<Fix18>), mat::Matrix<Fix18> x)
{
        double h = 0.0001;
        mat::Matrix<double> x_double(x.width, x.height, x.channel);
        for (int i; i < (x.width * x.height * x.channel); i++)
        {
                x_double.data[i] = x.data[i].to_double();
        }

        mat::Matrix<Fix18> grad(x.width, x.height, x.channel);

        for (int i; i < (x.width * x.height * x.channel); i++)
        {
                double tmp_val = x_double.data[i];

                //f(x + h)
                x_double.data[i] = tmp_val + h;
                double fxh1 = func(x);

                //f(x - h)
                x_double.data[i] = tmp_val - h;
                double fxh2 = func(x);

                grad.data[i] = (fxh1 - fxh2) / (2 * h);
                return grad;
        }
}
} // namespace grad