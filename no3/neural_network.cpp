#include <vector>
#include <map>

#include "../mnist_util/mnist_util.hpp"
#include "../matrix/matrix.hpp"
#include "../fixed_19/fixed_point.hpp"
#include "../activation/activate.hpp"
#include "../gradient/gradient.hpp"

class TowLayerNetwork
{

      public:
        std::map<std::string, mat::Matrix<Fix18>> *params;
        int xx;
        TowLayerNetwork(int input_size, int hidden_size, int output_size)
        {
                initializer(input_size, hidden_size, output_size, 0.01);
        }

        TowLayerNetwork(int input_size, int hidden_size, int output_size, double weight_init_std)
        {
                initializer(input_size, hidden_size, output_size, weight_init_std);
        }

        void initializer(int input_size, int hidden_size, int output_size, double weight_init_std)
        {
                this->xx = 10;

                this->params = new std::map<std::string, mat::Matrix<Fix18>>();
                int i = 0;
                mat::Matrix<Fix18> W1 = mat::Matrix<Fix18>(hidden_size, input_size).random() * weight_init_std;
                this->params->insert(std::make_pair("W1", W1));

                mat::Matrix<Fix18> b1 = mat::Matrix<Fix18>(hidden_size);
                this->params->insert(std::make_pair("b1", b1));

                mat::Matrix<Fix18> W2 = mat::Matrix<Fix18>(output_size, hidden_size).random() * weight_init_std;
                this->params->insert(std::make_pair("W2", W2));

                mat::Matrix<Fix18> b2 = mat::Matrix<Fix18>(output_size);
                this->params->insert(std::make_pair("b2", b2));

                std::cout << "XX" << this->xx << std::endl;
        }

        mat::Matrix<Fix18> predict(mat::Matrix<Fix18> x)
        {
                mat::Matrix<Fix18> a1 = x.dot(this->params->at("W1")) + this->params->at("b1");
                mat::Matrix<Fix18> z1 = a1.apply(sigmoid);
                mat::Matrix<Fix18> a2 = z1.dot(this->params->at("W2")) + this->params->at("b2");
                mat::Matrix<Fix18> y = softmax(a2);
                return y;
        }

        Fix18 loss(mat::Matrix<Fix18> x, mat::Matrix<Fix18> t)
        {
                mat::Matrix<Fix18> y = this->predict(x);
                Fix18 tmp = cross_entropy_error(y, t);
                return tmp;
        }

        Fix18 accuracy(mat::Matrix<Fix18> x, mat::Matrix<Fix18> t)
        {
                mat::Matrix<Fix18> y = predict(x);
                mat::Matrix<Fix18> y2 = y.max_arg(1);
                mat::Matrix<Fix18> t2 = t.max_arg(1);
        }

        std::map<std::string, mat::Matrix<Fix18>> gradient(mat::Matrix<Fix18> x, mat::Matrix<Fix18> t)
        {
                std::map<std::string, mat::Matrix<Fix18>> grads;
                int batch_num = x.width;

                /*****************************************forward*********************************/
                mat::Matrix<Fix18> a1 = x.dot(this->params->at("W1")) + this->params->at("b1");
                mat::Matrix<Fix18> z1 = a1.apply(sigmoid);
                mat::Matrix<Fix18> a2 = z1.dot(this->params->at("W2")) + this->params->at("b2");
                mat::Matrix<Fix18> y = softmax(a2);

                /*****************************************backward********************************/
                mat::Matrix<Fix18> y_minus_t = (y - t);
                mat::Matrix<double> y_minus_t_double(t.width, t.height, t.channel);

                //copy form Fix18 Matrix to double Matrix, to division
                for (int copy_cnt = 0; copy_cnt < (y_minus_t.width * y_minus_t.height * y_minus_t.channel); copy_cnt++)
                {
                        y_minus_t_double.data[copy_cnt] = y_minus_t.data[copy_cnt].to_double();
                }

                mat::Matrix<double> dy_double = y_minus_t_double / batch_num;
                mat::Matrix<Fix18> dy(dy_double.width, dy_double.height, dy_double.channel);

                //Write back from double Matrix to Fix18 Matrix
                for (int copy_cnt = 0; copy_cnt < (dy_double.width * dy_double.height * dy_double.channel); copy_cnt++)
                {
                        dy.data[copy_cnt] = dy_double.data[copy_cnt];
                }

                mat::Matrix<Fix18> W2 = z1.transpose().dot(dy);
                this->params->insert(std::make_pair("W2", W2));

                mat::Matrix<Fix18> b2 = dy.sum(0);
                this->params->insert(std::make_pair("b2", b2));

                mat::Matrix<Fix18> dz1 = dy.dot(params->at("W2").transpose());
                mat::Matrix<Fix18> da1 = a1.apply(sigmoid) * dz1;

                mat::Matrix<Fix18> W1 = x.transpose().dot(da1);
                this->params->insert(std::make_pair("W1", W1));

                mat::Matrix<Fix18> b1 = da1.sum(0);
                this->params->insert(std::make_pair("b1", b1));

                return grads;
        }
};

//hyper parameto

std::vector<double> train_los_list;
std::vector<double> train_acc_list;
std::vector<double> test_acc_list;

int main(void)
{
        //init
        mnist_nanager<Fix18> train_data, test_data;
        train_data.load("train-labels.idx1-ubyte", "train-images.idx3-ubyte");
        test_data.load("t10k-labels.idx1-ubyte", "t10k-images.idx3-ubyte");

        std::cout << "load ok" << std::endl;

        //parameter
        static int item_num = 10000;
        int train_size = train_data.length;
        static int batch_size = 100;
        static int learning_rate = 0.1;

        std::vector<Fix18> train_loss_list;
        std::vector<int> train_acc_list;
        std::vector<int> test_acc_list;

        int iter_per_epoch = ((train_size / batch_size) < 1) ? 1 : (train_size / batch_size);

        TowLayerNetwork testnet = TowLayerNetwork(784, 50, 10);

        std::cout << "network construction ok" << std::endl;

        for (int i = 0; i < item_num; i++)
        {
                //generate batch map
                std::vector<int> batch_map;
                std::random_device rnd;

                mat::Matrix<Fix18> batch_data(train_data.width * train_data.height, batch_size);
                mat::Matrix<Fix18> batch_label(train_data.width * train_data.height, 1);

                for (int rnd_cnt; rnd_cnt < batch_size; rnd_cnt++)
                {
                        int random_index = rnd() % train_size;
                        std::vector<int>::iterator cIter = std::find(batch_map.begin(), batch_map.end(), random_index);
                        if (cIter != batch_map.end())
                        {
                                //retry
                                //* std::cout << "retry" << std::endl;
                                rnd_cnt = rnd_cnt - 1;
                        }
                        else
                        {
                                //retry
                                //* std::cout << "rnd is " << random_index << std::endl;

                                batch_map.push_back(random_index);
                                batch_label.data[rnd_cnt] = train_data.label_array[random_index];
                                for (int copy_cnt = 0; copy_cnt < train_data.width * train_data.height; copy_cnt++)
                                {
                                        batch_data.data[batch_data.calc_pos(copy_cnt, rnd_cnt, 0)] = train_data.data_array[random_index].data[copy_cnt];
                                }
                        }
                }
                //*
                std::cout << "batch ok" << std::endl;

                //calcrate gradient;
                std::map<std::string, mat::Matrix<Fix18>> grads = testnet.gradient(batch_data, batch_label);
                //*
                std::cout << "gradient of" << std::endl;

                //refresh parametor

                testnet.params->at("W1") = testnet.params->at("W1") - (grads.at("W1") * learning_rate);
                testnet.params->at("b1") = testnet.params->at("b1") - (grads.at("b1") * learning_rate);
                testnet.params->at("W2") = testnet.params->at("W2") - (grads.at("W2") * learning_rate);
                testnet.params->at("b2") = testnet.params->at("b2") - (grads.at("b2") * learning_rate);

                //*
                std::cout << "refresh OK" << std::endl;

                Fix18 loss = testnet.loss(batch_data, batch_label);
                train_loss_list.push_back(loss);
                //calcrate accuary by epoch
                if (i % iter_per_epoch == 0)
                {
                        mat::Matrix<Fix18> test_batch_data(test_data.width * test_data.height, test_data.length);
                        mat::Matrix<Fix18> test_batch_label(test_data.length, 1);

                        for (int test_data_index = 0; test_data_index < test_data.length; test_data_index++)
                        {
                                test_batch_label.data[test_data_index] = test_data.label_array[i];
                                for (int copy_cnt = 0; copy_cnt < (test_data.width * test_data.height); copy_cnt++)
                                {
                                        test_batch_data.data[test_batch_data.calc_pos(test_data_index, copy_cnt, 0)] = test_data.data_array[test_data_index].data[copy_cnt];
                                }
                        }

                        double train_acc = testnet.accuracy(test_batch_data, test_batch_label).to_double();
                        double test_acc = testnet.accuracy(test_batch_data, test_batch_label).to_double();
                        train_acc_list.push_back(train_acc);
                        test_acc_list.push_back(test_acc);
                        std::cout << "train_acc :" << train_acc << "test_acc" << test_acc << std::endl;
                }
        }
}