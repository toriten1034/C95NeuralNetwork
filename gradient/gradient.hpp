#include "../matrix/matrix.hpp"
#include "../fixed_19/fixed_point.hpp"

void save_matrix(mat::Matrix<Fix18> src, std::string filename)
{
        assert(src.channel > 1);
        std::fstream fs;
        fs.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);

        for (int y = 0; y < src.height; y++)
        {
                for (int x = 0; x < src.width; x++)
                {
                        fs << src.data[src.calc_pos(x, y, 0)] << " ";
                }
                if (y < src.height - 1)
                        fs << "\n";
        }
        fs.close();
}

namespace grad
{
mat::Matrix<double> numerical_gradient(double (*func)(mat::Matrix<double>), mat::Matrix<Fix18> x)
{
        double h = 0.0001;
        mat::Matrix<double> x_double(x.width, x.height, x.channel);
        for (int i = 0; i < (x.width * x.height * x.channel); i++)
        {
                x_double.data[i] = x.data[i].to_double();
        }

        mat::Matrix<double> grad(x.width, x.height, x.channel);

        for (int i = 0; i < (x.width * x.height * x.channel); i++)
        {
                double tmp_val = x_double.data[i];

                //f(x + h)
                x_double.data[i] = tmp_val + h;
                double fxh1 = func(x_double);

                //f(x - h)
                x_double.data[i] = tmp_val - h;
                double fxh2 = func(x_double);

                x_double.data[i] = tmp_val;

                grad.data[i] = (fxh1 - fxh2) / (2 * h);
        }
        return grad;
}
mat::Matrix<Fix18> numerical_gradient_Fix18(double (*func)(mat::Matrix<double>), mat::Matrix<Fix18> x)
{
        double h = 0.0001;
        mat::Matrix<double> x_double(x.width, x.height, x.channel);
        for (int i = 0; i < (x.width * x.height * x.channel); i++)
        {
                x_double.data[i] = x.data[i].to_double();
        }

        mat::Matrix<Fix18> grad(x.width, x.height, x.channel);

        for (int i = 0; i < (x.width * x.height * x.channel); i++)
        {
                double tmp_val = x_double.data[i];

                //f(x + h)
                x_double.data[i] = tmp_val + h;
                double fxh1 = func(x_double);

                //f(x - h)
                x_double.data[i] = tmp_val - h;
                double fxh2 = func(x_double);

                x_double.data[i] = tmp_val;

                grad.data[i] = Fix18((fxh1 - fxh2) / (2 * h));
        }
        return grad;
}
} // namespace grad