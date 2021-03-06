#include <vector>
#include <algorithm>
#include <map>

#include "../mnist_util/mnist_util.hpp"
#include "../matrix/matrix.hpp"
#include "activate.hpp"
#include "../activation/activate.hpp"
#include "../gradient/gradient.hpp"

#define VAL_TYPE Fix18

mat::Matrix<Fix18> d2F(mat::Matrix<double> src)
{
	mat::Matrix<Fix18> result(src.width, src.height, src.channel);
	for (int i = 0; i < src.width * src.height * src.channel; i++)
	{
		result.data[i] = src.data[i];
	}
	return result;
}

mat::Matrix<double> F2d(mat::Matrix<Fix18> src)
{
	mat::Matrix<double> result(src.width, src.height, src.channel);
	for (int i = 0; i < src.width * src.height * src.channel; i++)
	{
		result.data[i] = src.data[i].to_double();
	}
	return result;
}

class TwoLayerNetwork
{

  public:
	std::map<std::string, mat::Matrix<double>> *params;
	int xx;
	TwoLayerNetwork(int input_size, int hidden_size, int output_size)
	{
		initializer(input_size, hidden_size, output_size, 0.01);
	}

	TwoLayerNetwork(int input_size, int hidden_size, int output_size, double weight_init_std)
	{
		initializer(input_size, hidden_size, output_size, weight_init_std);
	}

	void initializer(int input_size, int hidden_size, int output_size, double weight_init_std)
	{

		this->params = new std::map<std::string, mat::Matrix<double>>();
		int i = 0;
		mat::Matrix<double> W1 = mat::Matrix<double>(hidden_size, input_size).random(0, 0.5) * weight_init_std;
		this->params->insert(std::make_pair("W1", W1));

		mat::Matrix<double> b1 = mat::Matrix<double>(hidden_size);
		this->params->insert(std::make_pair("b1", b1));

		mat::Matrix<double> W2 = mat::Matrix<double>(output_size, hidden_size).random(0, 0.5) * weight_init_std;
		this->params->insert(std::make_pair("W2", W2));

		mat::Matrix<double> b2 = mat::Matrix<double>(output_size);
		this->params->insert(std::make_pair("b2", b2));
	}

	mat::Matrix<double> predict(mat::Matrix<double> x)
	{

		//b1 has error
		mat::Matrix<double> a1 = x.dot(this->params->at("W1")) + this->params->at("b1");

		mat::Matrix<double> z1 = a1.apply(sigmoid);
		mat::Matrix<double> a2 = z1.dot(this->params->at("W2")) + this->params->at("b2");
		mat::Matrix<double> y = softmax(a2);
		return y;
	}

	mat::Matrix<double> predictFix18(mat::Matrix<double> x)
	{

		//b1 has error
		mat::Matrix<Fix18> a1 = d2F(x).dot(d2F(this->params->at("W1"))) + d2F(this->params->at("b1"));

		mat::Matrix<Fix18> z1 = a1.apply(sigmoid);
		mat::Matrix<Fix18> a2 = z1.dot(d2F(this->params->at("W2"))) + d2F(this->params->at("b2"));
		mat::Matrix<Fix18> y = softmax(a2);
		return F2d(y);
	}

	double loss(mat::Matrix<double> x, mat::Matrix<double> t)
	{
		mat::Matrix<double> y = this->predict(x);
		double tmp = cross_entropy_error(y, t);
		return tmp;
	}
	double accuracy(mat::Matrix<double> x, mat::Matrix<double> t)
	{
		assert(x.width == t.width || x.height == t.height);
		mat::Matrix<double> y = this->predict(x);
		mat::Matrix<double> y2 = y.max_arg(0);
		mat::Matrix<double> t2 = t;

		mat::Matrix<double> equal = y2.compare(t2);
		//equal.show();

		long double sum = 0;
		for (int i = 0; i < t.width * t.height * t.channel; i++)
		{
			sum += equal.data[i];
		}
		double result = sum / (double)x.height;
		/*   if (result > 0.8)
                {
                        y2.transpose().show();
                        t2.transpose().show();
                }*/
		// std::cout << result << ":" << sum << std::endl;

		return result;
	}

	double accuracyFix18(mat::Matrix<double> x, mat::Matrix<double> t)
	{
		assert(x.width == t.width || x.height == t.height);
		mat::Matrix<double> y = this->predictFix18(x);
		mat::Matrix<double> y2 = y.max_arg(0);
		mat::Matrix<double> t2 = t;

		mat::Matrix<double> equal = y2.compare(t2);
		//equal.show();

		long double sum = 0;
		for (int i = 0; i < t.width * t.height * t.channel; i++)
		{
			sum += equal.data[i];
		}
		double result = sum / (double)x.height;
		/*   if (result > 0.8)
                {
                        y2.transpose().show();
                        t2.transpose().show();
                }*/
		// std::cout << result << ":" << sum << std::endl;

		return result;
	}

	std::map<std::string, mat::Matrix<double>> gradient(mat::Matrix<double> x, mat::Matrix<double> t)
	{
		std::map<std::string, mat::Matrix<double>> grads;
		int batch_num = x.height;

		/*****************************************forward*********************************/
		mat::Matrix<double> a1 = x.dot(this->params->at("W1")) + this->params->at("b1");
		mat::Matrix<double> z1 = a1.apply(sigmoid);
		mat::Matrix<double> a2 = z1.dot(this->params->at("W2")) + this->params->at("b2");
		mat::Matrix<double> y = softmax(a2);

		/*****************************************backward********************************/
		mat::Matrix<double> y_minus_t = (y - t);

		mat::Matrix<double> y_minus_t_double(t.width, t.height, t.channel);

		//copy form Fix18 Matrix to double Matrix, to division
		for (int copy_cnt = 0; copy_cnt < (y_minus_t.width * y_minus_t.height * y_minus_t.channel); copy_cnt++)
		{
			y_minus_t_double.data[copy_cnt] = y_minus_t.data[copy_cnt];
		}

		mat::Matrix<double> dy_double = y_minus_t_double / (double)batch_num;
		mat::Matrix<double> dy(dy_double.width, dy_double.height, dy_double.channel);

		//Write back from double Matrix to Fix18 Matrix
		for (int copy_cnt = 0; copy_cnt < (dy_double.width * dy_double.height * dy_double.channel); copy_cnt++)
		{
			dy.data[copy_cnt] = dy_double.data[copy_cnt];
		}

		mat::Matrix<double> W2 = z1.transpose().dot(dy);
		grads.insert(std::make_pair("W2", W2));

		mat::Matrix<double> b2 = dy.sum(1);
		grads.insert(std::make_pair("b2", b2));

		mat::Matrix<double> dz1 = dy.dot(params->at("W2").transpose());
		mat::Matrix<double> da1 = a1.apply(sigmoid_grad) * dz1;
		mat::Matrix<double> W1 = x.transpose().dot(da1);
		grads.insert(std::make_pair("W1", W1));

		mat::Matrix<double> b1 = da1.sum(1);

		//         std::cout << " neko da1" << std::endl;
		//        da1.show();

		grads.insert(std::make_pair("b1", b1));

		return grads;
	}
	void save_txt(std::string filename, mat::Matrix<Fix18> save_data)
	{

		std::ofstream fs;
		fs.open(filename);
		fs << save_data.width << "," << save_data.height << "," << save_data.channel << "\n";
		for (int i = 0; i < save_data.width * save_data.height * save_data.channel; i++)
		{
			fs << save_data.data[i].value << ",";
		}
		fs.close();
	}
	void save_all_txt(std::string tag)
	{
		save_txt(tag + "_W1.txt", d2F(this->params->at("W1")));
		save_txt(tag + "_b1.txt", d2F(this->params->at("b1")));
		save_txt(tag + "_W2.txt", d2F(this->params->at("W2")));
		save_txt(tag + "_b2.txt", d2F(this->params->at("b2")));
	}
};

//hyper parameto

std::vector<double> train_los_list;
std::vector<double> train_acc_list;
std::vector<double> test_acc_list;

int main(void)
{

	mnist_nanager<double> train_data, test_data;
	train_data.load("../MNIST/train-labels.idx1-ubyte", "../MNIST/train-images.idx3-ubyte");
	test_data.load("../MNIST/t10k-labels.idx1-ubyte", "../MNIST/t10k-images.idx3-ubyte");

	//  std::cout << "load ok" << std::endl;

	//parameter
	static int iters_num = 10000;
	int train_size = train_data.length;
	static int batch_size = 100;
	static double learning_rate = 0.1;

	std::vector<double> train_loss_list;
	std::vector<int> train_acc_list;
	std::vector<int> test_acc_list;

	int iter_per_epoch = ((train_size / batch_size) < 1) ? 1 : (train_size / batch_size);

	TwoLayerNetwork testnet = TwoLayerNetwork(784, 50, 10);

	std::cout << "network construction ok" << std::endl;

	mat::Matrix<double> test_batch_data(test_data.width * test_data.height, 1000);
	mat::Matrix<double> test_batch_label(1, 1000);

	for (int test_data_index = 0; test_data_index < 1000; test_data_index++)
	{
		test_batch_label.data[test_data_index] = (double)test_data.label_array[test_data_index];

		if (test_batch_label.data[test_data_index] == 4)
		{
			for (int copy_cnt = 0; copy_cnt < (test_data.width * test_data.height); copy_cnt++)
			{
				Fix18 tmp = test_data.data_array[test_data_index].data[copy_cnt];
				std::cout << tmp.value << ",";
			}
			return 0;
		}
		for (int copy_cnt = 0; copy_cnt < (test_data.width * test_data.height); copy_cnt++)
		{
			test_batch_data.data[test_batch_data.calc_pos(copy_cnt, test_data_index, 0)] = test_data.data_array[test_data_index].data[copy_cnt];
		}
	}

	for (int i = 0; i < iters_num; i++)
	{
		//generate batch map
		std::vector<int> batch_map;
		std::random_device rnd;

		mat::Matrix<double> batch_data(train_data.width * train_data.height, batch_size);
		mat::Matrix<double> batch_label(10, batch_size);
		mat::Matrix<double> batch_label_eval(1, batch_size);

		for (int rnd_cnt = 0; rnd_cnt < batch_size; rnd_cnt++)
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
				batch_label_eval.data[rnd_cnt] = train_data.label_array[random_index];

				batch_label.data[batch_label.calc_pos(train_data.label_array[random_index], rnd_cnt, 0)] = (double)1;
				for (int copy_cnt = 0; copy_cnt < train_data.width * train_data.height; copy_cnt++)
				{
					batch_data.data[batch_data.calc_pos(copy_cnt, rnd_cnt, 0)] = train_data.data_array[random_index].data[copy_cnt];
				}
			}
		}
		double train_acc = testnet.accuracy(batch_data, batch_label_eval);
		std::cout << "  train_acc0 " << train_acc << std::endl;

		//*
		//              std::cout << "batch ok" << std::endl;

		//calcrate gradient;
		std::map<std::string, mat::Matrix<double>> grads = testnet.gradient(batch_data, batch_label);
		//*
		//                std::cout << "gradient ok" << std::endl;

		testnet.params->at("W1") = testnet.params->at("W1") - (grads.at("W1") * learning_rate);
		testnet.params->at("b1") = testnet.params->at("b1") - (grads.at("b1") * learning_rate);
		testnet.params->at("W2") = testnet.params->at("W2") - (grads.at("W2") * learning_rate);
		testnet.params->at("b2") = testnet.params->at("b2") - (grads.at("b2") * learning_rate);

		//testnet.params->at("W1").show();

		double loss = testnet.loss(batch_data, batch_label);
		train_loss_list.push_back(loss);

		//*
		//    std::cout << "loss OK" << std::endl;

		if (i % iter_per_epoch == 0)
		{

			double train_acc = testnet.accuracyFix18(batch_data, batch_label_eval);
			double test_acc = testnet.accuracyFix18(test_batch_data, test_batch_label);
			train_acc_list.push_back(train_acc);
			test_acc_list.push_back(test_acc);
			std::cout << "\ttrain_acc" << train_acc << std::endl;
			std::cout << "\ttest_acc" << test_acc << std::endl;

			if (test_acc > 0.85)
			{
				std::cout << "clear" << std::endl;
				std::ostringstream stream;
				stream << "test:" << test_acc;
				testnet.save_all_txt(stream.str());
			}
		}
	}
}