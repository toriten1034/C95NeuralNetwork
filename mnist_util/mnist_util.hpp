/************************************************************************
 * Matrix class 
 * This class for matrix calcration.
 * 
 * Rules
 * Do not make void function of Matrix calcration.
 * ***********************************************************************/

#include "../matrix/matrix.hpp"
#include "../fixed_19/fixed_point.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

#ifndef INCLUDE_MNIST_UTIL
#define INCLUDE_MNIST_UTIL

template <typename TYPE>
class mnist_nanager
{

  public:
	//sizes
	int width;
	int height;
	int length;

	int *label_array;
	std::vector<mat::Matrix<TYPE>> data_array;

	/************************************************
         * Load MNIST Files
         * 
         * arguments
         * label_path: label_file path
         * data_path: data file path
         * 
         * if finish correctry return 0, else return -1
         * *********************************************/
	int load(char *label_path, char *data_path)
	{
		std::fstream label_fs;
		std::fstream data_fs;

		label_fs.open(label_path, std::ios::in | std::ios::binary);
		data_fs.open(data_path, std::ios::in | std::ios::binary);

		//open check
		if (!label_fs.is_open())
		{
			std::cerr << "label file open error : " << label_path << std::endl;
			return -1;
		}

		if (!data_fs.is_open())
		{
			std::cerr << "data file open error : " << data_path << std::endl;
			return -1;
		}

		//identifier check
		int label_id = 0;
		int data_id = 0;

		label_fs.read((char *)&label_id, sizeof(label_id));
		label_id = little2big(label_id);

		data_fs.read((char *)&data_id, sizeof(data_id));
		data_id = little2big(data_id);

		if (label_id != 2049)
		{
			std::cerr << "label identifier is " << label_id << " worng label file" << std::endl;
			return -1;
		}

		if (data_id != 2051)
		{
			std::cerr << "data indentirier is " << data_id << " worng data file" << std::endl;
			return -1;
		}

		//size check
		int label_size, data_size;
		label_fs.read((char *)&label_size, sizeof(int));
		label_size = little2big(label_size);

		data_fs.read((char *)&data_size, sizeof(int));
		data_size = little2big(data_size);

		if (label_size != data_size)
		{
			std::cerr << "data size not match" << std::endl;
			return -1;
		}
		length = label_size;

		//read width an heith
		data_fs.read((char *)&height, sizeof(int));
		data_fs.read((char *)&width, sizeof(int));

		height = little2big(height);
		width = little2big(width);
		//type ckeck

		label_array = (int *)malloc(sizeof(int) * length);

		//load labels and images
		int type;
		int label;

		for (int i = 0; i < length || (!label_fs.eof() && !data_fs.eof()); i++)
		{
			unsigned char label_tmp;
			label_fs.read((char *)&label_tmp, sizeof(label_tmp));
			label_array[i] = label_tmp;

			mat::Matrix<TYPE> tmp_mat(width, height);
			for (int j = 0; j < width * height; j++)
			{
				unsigned char data_tmp;
				data_fs.read((char *)&data_tmp, sizeof(data_tmp));
				tmp_mat.data[j] = (double)data_tmp / 256.0;
			}
			data_array.push_back(tmp_mat);
		}
		return 0;
	}
	//show containered data status
	void show_status(void)
	{
		std::cout << "width:" << width << std::endl;
		std::cout << "height:" << height << std::endl;
		std::cout << "length:" << length << std::endl;
	}
	/**************************************************
         * output truth
         * args 
         * index: index of truth to calclate cross entropy error
         * *********************************************/
	mat::Matrix<TYPE> truth(int index)
	{
		mat::Matrix<Fix18> answer_mat(10, 1);
		int answer = label_array[index];
		answer_mat.data[answer] = (double)1.0;
		return answer_mat;
	}

  private:
	/***********************************
       * This function converts endians
       * args
       * x: data to convert endian
       * *********************************/
	int little2big(int x)
	{
		int tmp = ((x & 0xFF000000) >> 24) | ((x & 0x00FF0000) >> 8) | ((x & 0x0000FF00) << 8) | ((x & 0x000000FF) << 24);
		return tmp;
	}
};

#endif