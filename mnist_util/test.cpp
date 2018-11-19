#include "mnist_util.hpp"
#include "../fixed_19/fixed_point.hpp"

class TowLayerNetwork
{
        mat::Matrix<Fix18> W1, b1, W2, b2;
        TowLayerNetwork(int input_size, int hidden_size, int output_size, int weight_init_std)
        {
                W1 = weight_init_std * mat::Matrix<Fix18>(input_size, hidden_size).random_init();
                b1 = mat::Matrix<Fix18>(hidden_size);
                W2 = weight_init_std * mat::Matrix<Fix18>(hidden_size, output_size).random_init();
                b2 = mat::Matrix<Fix18>(output_size);
        }

        predict(mat::Matrix<Fix18> x)
        {
                mat::Matrix<Fix18> a1 = x.dot(W1) + b1;
                mat::Matrix<Fix18> z1 = a1.apply(relu);
                mat::Matrix<Fix18> a2 = z1.dot(W2) + b2;
                mat::Matrix<Fix18> y = softmax(a2);
                return y;
        }

        int loss(mat::Matrix<Fix18> x, mat::Matrix<Fix18> t)
        {
                mat::Matrix<Fix18> y = predict(x);
                return
        }

} int main(void)
{
        mnist_nanager<Fix18> train_data;
        char *label_file_name = "train-labels.idx1-ubyte";
        char *data_file_name = "train-images.idx3-ubyte";
        train_data.load(label_file_name, data_file_name);
        train_data.show_status();

        train_data.data_array[0].show();
        return 0;
}