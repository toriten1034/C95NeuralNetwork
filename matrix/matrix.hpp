#include <stdlib.h>
#include <random>
#include <cassert>
#include <iostream>

#ifndef INCLUDE_MATRIX
#define INCLUDE_MATRIX

namespace mat
{
template <typename TYPE>
class Matrix
{

      private:
	int cnt = 0;

      public:
	TYPE *data;
	int height = 0; //if height == 0 then dimension y is zero
	int width = 0;
	int channel = 0;

	/******************************************************************************************
	 * Calcrate index from x,y,x axis
	 * ***************************************************************************************/
	int calc_pos(int x, int y, int d)
	{
		int pos = 0;
		pos += d * (width * height);
		pos += y * width;
		pos += x;
		return pos;
	}

	// Constructor Create Matrix Instance
	void initialize(int width, int height, int channel)
	{
		assert(channel > 0 && height > 0 && width > 0);
		this->channel = channel;
		this->height = height;
		this->width = width;
		this->data = (TYPE *)calloc(sizeof(TYPE), width * height * channel);
	}

	/******************************************************************************************
	 * Create 1D Matrix data, like a vector
	 *  args
	 * 	width vector length
	 * ***************************************************************************************/
	Matrix(int width)
	{
		assert(width > 0);
		initialize(width, 1, 1);
	}
	/******************************************************************************************
	 * Create 2D Matrix data
	 *  args
	 * 	width : matrix width
	 * 	height : matrix height
	 * ***************************************************************************************/
	Matrix(int width, int height)
	{
		assert(width > 0 && height > 0);
		initialize(width, height, 1);
	}

	/******************************************************************************************
	 * Create 3D Matrix data
	 *  args
	 * 	width : matrix width
	 * 	height : matrix height
	 * 	channel : matrix channel, in other word matrix depth
	 * ***************************************************************************************/
	Matrix(int width, int height, int channel)
	{
		assert(width > 0 && height > 0 && channel > 0);
		initialize(width, height, channel);
	}

	//Copy constructor
	Matrix(const Matrix &obj)
	{
		this->width = obj.width;
		this->height = obj.height;
		this->channel = obj.channel;
		this->data = data = (TYPE *)calloc(sizeof(TYPE), obj.width * obj.height * obj.channel);
		for (int i = 0; i < (obj.width * obj.height * obj.channel); i++)
		{
			this->data[i] = obj.data[i];
		}
	}

	//Copy
	void operator=(const Matrix<TYPE> &obj)
	{
		this->width = obj.width;
		this->height = obj.height;
		this->channel = obj.channel;
		if (this->data != NULL)
		{
			free(this->data);
		}
		this->data = data = (TYPE *)calloc(sizeof(TYPE), obj.width * obj.height * obj.channel);
		for (int i = 0; i < (obj.width * obj.height * obj.channel); i++)
		{
			this->data[i] = obj.data[i];
		}
	}

	//Destructor
	~Matrix()
	{
		if (this->data != NULL)
		{
			free(this->data);
			this->data = NULL;
		}
	}

	// Reshapes configure aspects

	/******************************************************************************************
	 * Convert to 1D Matrix data, like a vector
	 * args
	 * 	width : vectot lengs
	 * return 
	 *	reshaped Matrix 
	 * ***************************************************************************************/
	Matrix<TYPE> reshape(int width)
	{
		assert(this->width != 0 && this->height != 0 && this->channel != 0);
		asser((this->channel * this->height * this->width) == width);

		Matrix<TYPE> tmp(width, 1, 1);
		for (int i = 0; i < this->width * this->height * this->channel; i++)
			tmp.data[i] = this->data[i];

		return tmp;
	}

	/******************************************************************************************
	 * Convert to 2D Matrix data
	 *  args
	 * 	width : matrix width
	 * 	height : matrix height
	 * return
	 *	reshaped Matrix 
	 * ***************************************************************************************/
	Matrix<TYPE> reshape(int width, int height)
	{
		assert(this->width != 0 && this->height != 0 && this->channel != 0);
		assert((this->channel * this->height * this->width) == width * height);

		Matrix<TYPE> tmp(width, height, 1);
		for (int i = 0; i < width * height * channel; i++)
			tmp.data[i] = this->data[i];

		return tmp;
	}

	/******************************************************************************************
	 * Convert to 3D Matrix data
	 *  args
	 * 	width : matrix width
	 * 	height : matrix height
	 * 	channel : matrix channel, in other word matrix depth
	 * return
	 *	reshaped Matrix 
	 * ***************************************************************************************/
	Matrix<TYPE> reshape(int width, int height, int channel)
	{
		assert(this->width != 0 && this->height != 0 && this->channel != 0);
		assert((this->channel * this->height * this->width) == (channel * width * height));

		Matrix<TYPE> tmp(width, height, channel);
		for (int i = 0; i < width * height * channel; i++)
			tmp.data[i] = this->data[i];

		return tmp;
	}

	/******************************************************************************************
	 * Opt out Matrix datas
	 * ***************************************************************************************/
	void show(void)
	{
		for (int i = 0; i < this->channel; i++)
		{
			if (i > 1)
				std::cout << "channel " << i << std::endl;
			std::cout << "[";
			for (int j = 0; j < this->height; j++)
			{
				std::cout << "[";
				for (int k = 0; k < this->width; k++)
				{
					int ptr = (i * this->width * this->height) + (j * this->width) + k;
					std::cout << data[ptr] << " ";
				}
				std::cout << "]";
				if (j < this->height - 1)
				{
					std::cout << std::endl;
				}
			}
			std::cout << "]" << std::endl;
		}
	}

	/******************************************************************************************
	 * opt out Matrix Dimensions Lengths
	 * ***************************************************************************************/
	void shape(void)
	{
		if (this->height == 0 || this->channel == 0)
		{
			std::cout << this->width << std::endl;
		}
		else if (this->channel == 0)
		{
			std::cout << this->width << "," << this->height << std::endl;
		}
		else
		{
			std::cout << this->width << "," << this->height << "," << this->channel << std::endl;
		}
	}

	/******************************************************************************************
	 * Calcate Innner Product
	 * args
	 * 	obj : same type matrix.   A.dot(B) is AB
	 * return 
	 * 	Matrix of innner product matrix
	 * ***************************************************************************************/

	Matrix<TYPE> dot(Matrix<TYPE> obj)
	{
		assert(this->channel == obj.channel);
		assert(this->width == obj.height);
		//avoid null
		assert((0 < this->width && 0 < this->height) || (0 < obj.width && 0 < obj.height));

		if ((this->height == 0) || (obj.height == 0))
		{
			//multiplying condition
			assert(this->width == obj.width);
			Matrix<TYPE> sum(1);
			for (int i = 0; i < width; i++)
			{
				sum.data[0] = sum.data[0] + this->data[i] * obj.data[i];
			}
			return sum;
		}
		else
		{
			//multiplying condition
			assert((0 < this->width || 0 < this->height) || (0 < obj.width || 0 < obj.height));
			assert(this->width == obj.height);

			Matrix<TYPE> result(obj.width, this->height, this->channel);

			for (int i = 0; i < result.channel; i++)
			//channel
			{
				int channel_offset = (result.width * result.height) * i;
				for (int j = 0; j < result.height; j++)
				//height
				{
					int line_offset = j * result.width;

					for (int k = 0; k < result.width; k++)
					//width
					{
						//line inner
						TYPE sum = 0.0;
						for (int l = 0; l < this->width; l++)
						{
							int pt_this = l + j * this->width;
							int pt_obj = l * obj.width + k;
							//	std::cout << this->data[pt_this] << "*" << obj.data[pt_obj] << std::endl;
							sum = sum + this->data[pt_this] * obj.data[pt_obj];
						}
						// std::cout << channel_offset + line_offset + k << "=" << sum << std::endl;
						result.data[channel_offset + line_offset + k] = sum;
					}
				}
			}
			return result;
		}
	}

	/******************************************************************************************
	 * Calcrate self transpose
	 * return 
	 * 	transposed Matrix
	 * ***************************************************************************************/
	Matrix<TYPE> transpose(void)
	{
		//only 2d array
		assert(channel == 1);
		Matrix<TYPE> result(this->height, this->width, this->channel);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				result.data[result.calc_pos(j, i, 0)] = this->data[this->calc_pos(i, j, 0)];
			}
		}
		return result;
	}

	/******************************************************************************************
	 * Aplly argment function to all elements
	 * return 
	 * 	Applyed Element Matrix
	 * ***************************************************************************************/
	Matrix<TYPE> apply(TYPE (*func)(TYPE))
	{
		Matrix<TYPE> result(this->width, this->height, this->channel);
		for (int i = 0; i < width * height * channel; i++)
		{
			result.data[i] = func(this->data[i]);
		}
		return result;
	}

	/******************************************************************************************
	 * Pic up max value from matrix
	 * arg
	 * 	axis direction of pic up line or face, this data dimention is lower 1 this Matrix
	 * 
	 *  	If this matrix's dimention is 1, then,,
	 * 		 result is 1 element matrix;
	 *  	If this matrix's dimention is 2, then...
	 * 		if axis is 0, pic up veatrical max value array.
	 *  		else if axis is 1, pic up horizonal max value attai,
	 *   	If this matrix's dimention is 3 ,then...
 	 * 		if axis is 0, pic up veatrical max value face.
	 *  		else if axis is 1, pic up horizonal max value face.
	 * 		else if axis is 2, pic up depth nax value face. 
	 * ***************************************************************************************/

	Matrix<TYPE> max(int axis)
	{
		assert(this->width != 0 && this->height != 0 && this->channel != 0);
		if ((width == 1 || height == 1) && channel == 1)
		{
			Matrix<TYPE> result(1, 1, 1);
			for (int i = 0; i < (this->width * this->height * this->channel); i++)
			{
				if (result.data[0] < this->data[i])
				{
					result.data[0] = this->data[i];
				}
			}
			return result;
		}
		else if (channel == 1)
		{
			if (axis == 0)
			//row direction
			{
				Matrix<TYPE> result(1, this->height);
				for (int y = 0; y < this->height; y++)
				{
					for (int i = 0; i < this->width; i++)
					{
						if (result.data[result.calc_pos(0, y, 0)] < this->data[calc_pos(i, y, 0)])
						{
							result.data[result.calc_pos(0, y, 0)] = this->data[calc_pos(i, y, 0)];
						}
					}
				}
				return result;
			}
			else if (axis == 1)
			//colmn direction
			{
				Matrix<TYPE> result(this->width, 1);
				for (int i = 0; i < this->height; i++)
				{
					for (int x = 0; x < this->height; x++)
					{
						if (result.data[result.calc_pos(x, 0, 0)] < this->data[calc_pos(x, i, 0)])
						{
							result.data[result.calc_pos(x, 0, 0)] = this->data[calc_pos(x, i, 0)];
						}
					}
				}
				return result;
			}
		}
		else if (1 < channel)
		{
			if (axis == 0)
			//row direction
			{
				Matrix<TYPE> result(1, this->height, this->channel);
				for (int i = 0; i < this->width; i++)
				{
					for (int y = 0; y < this->height; y++)
					{
						for (int z = 0; z < this->channel; z++)
						{
							if (result.data[result.calc_pos(0, y, z)] < this->data[calc_pos(i, y, z)])
							{
								result.data[result.calc_pos(0, y, z)] = this->data[calc_pos(i, y, z)];
							}
						}
					}
				}
				return result;
			}

			else if (axis == 1)
			//column direction
			{
				Matrix<TYPE> result(this->width, 1, this->channel);
				for (int i = 0; i < this->height; i++)
				{
					for (int x = 0; x < this->height; x++)
					{
						for (int z = 0; z < this->channel; z++)
						{
							if (result.data[result.calc_pos(x, 0, z)] < this->data[calc_pos(x, i, z)])
							{
								result.data[result.calc_pos(x, 0, z)] = this->data[calc_pos(x, i, z)];
							}
						}
					}
				}
				return result;
			}
			else if (axis == 2)
			//depth direction
			{
				Matrix<TYPE> result(this->width, this->height, 1);

				for (int i = 0; i < this->channel; i++)
				{
					for (int x = 0; x < this->height; x++)
					{
						for (int y = 0; y < this->channel; y++)
						{
							if (result.data[result.calc_pos(x, y, 0)] < this->data[calc_pos(x, y, i)])
							{
								result.data[result.calc_pos(x, y, 0)] = this->data[calc_pos(x, y, i)];
							}
						}
					}
				}
				return result;
			}
		}
	}

	/******************************************************************************************
	 * Pic up index of max value from matrix
	 * arg
	 * 	axis direction of pic up line or face, this data dimention is lower 1 this Matrix
	 * 
	 *  	If this matrix's dimention is 1, then,,
	 * 		 result is 1 element matrix;
	 *  	If this matrix's dimention is 2, then...
	 * 		if axis is 0, pic up veatrical index  array of  max values.
	 *  		else if axis is 1, pic up horizonal index array of max values,
	 *   	If this matrix's dimention is 3 ,then...
 	 * 		if axis is 0, pic up veatrical index face of max values.
	 *  		else if axis is 1, pic up horizonal index face of max values.
	 * 		else if axis is 2, pic up depth index face of max values. 
	 * ***************************************************************************************/
	Matrix<TYPE> max_arg(int axis)
	{
		assert(height != 0 && channel != 0);
		if ((width == 0 || height == 0) && channel == 1)
		{
			Matrix<TYPE> result(1, 1, 1);
			Matrix<TYPE> result_index(1, 1, 1);
			for (int i = 0; i < (this->width * this->height * this->channel); i++)
			{
				if (result.data[0] < this->data[i])
				{
					result.data[0] = this->data[i];
					result_index.data[0] = this->data[i];
				}
			}
			return result_index;
		}
		else if (channel == 1)
		{
			if (axis == 0)
			//row direction
			{
				Matrix<TYPE> result(1, this->height);
				Matrix<TYPE> result_index(1, this->height);
				for (int i = 0; i < this->width; i++)
				{
					for (int y = 0; y < this->height; y++)
					{
						if (result.data[result.calc_pos(0, y, 0)] < this->data[calc_pos(i, y, 0)])
						{
							result.data[result.calc_pos(0, y, 0)] = this->data[calc_pos(i, y, 0)];
							result_index.data[result_index.calc_pos(0, y, 0)] = calc_pos(i, y, 0);
						}
					}
				}
				return result_index;
			}
			else if (axis == 1)
			//colmn direction
			{
				Matrix<TYPE> result(this->width, 1);
				Matrix<TYPE> result_index(this->width, 1);
				for (int i = 0; i < this->height; i++)
				{
					for (int x = 0; x < this->height; x++)
					{
						if (result.data[result.calc_pos(x, 0, 0)] < this->data[calc_pos(x, i, 0)])
						{
							result.data[result.calc_pos(x, 0, 0)] = this->data[calc_pos(x, i, 0)];
							result_index.data[result_index.calc_pos(x, 0, 0)] = calc_pos(x, i, 0);
						}
					}
				}
				return result_index;
			}
		}
		else if (1 < channel)
		{
			if (axis == 0)
			//row direction
			{
				Matrix<TYPE> result(1, this->height, this->channel);
				Matrix<TYPE> result_index(1, this->height, this->channel);
				for (int i = 0; i < this->width; i++)
				{
					for (int y = 0; y < this->height; y++)
					{
						for (int z = 0; z < this->channel; z++)
						{
							if (result.data[result.calc_pos(0, y, z)] < this->data[calc_pos(i, y, z)])
							{
								result.data[result.calc_pos(0, y, z)] = this->data[calc_pos(i, y, z)];
								result_index.data[result_index.calc_pos(0, y, z)] = calc_pos(i, y, z);
							}
						}
					}
				}
				return result_index;
			}

			else if (axis == 1)
			//column direction
			{
				Matrix<TYPE> result(this->width, 1, this->channel);
				Matrix<TYPE> result_index(this->width, 1, this->channel);
				for (int i = 0; i < this->height; i++)
				{
					for (int x = 0; x < this->height; x++)
					{
						for (int z = 0; z < this->channel; z++)
						{
							if (result.data[result.calc_pos(x, 0, z)] < this->data[calc_pos(x, i, z)])
							{
								result.data[result.calc_pos(x, 0, z)] = this->data[calc_pos(x, i, z)];
								result_index.data[result_index.calc_pos(x, 0, z)] = calc_pos(x, i, z);
							}
						}
					}
				}
				return result_index;
			}
			else if (axis == 2)
			//depth direction
			{
				Matrix<TYPE> result(this->width, this->height, 1);
				Matrix<TYPE> result_index(this->width, this->height, 1);

				for (int i = 0; i < this->channel; i++)
				{
					for (int x = 0; x < this->height; x++)
					{
						for (int y = 0; y < this->channel; y++)
						{
							if (result.data[result.calc_pos(x, y, 0)] < this->data[calc_pos(x, y, i)])
							{
								result.data[result.calc_pos(x, y, 0)] = this->data[calc_pos(x, y, i)];
								result_index.data[result_index.calc_pos(x, y, 0)] = calc_pos(x, y, i);
							}
						}
					}
				}
				return result_index;
			}
		}
	}

	/******************************************************************************************
	 * Calcrate sum of matrix
	 * arg
	 * 	axis : direction of calcrating sums line or face, this data dimention is lower 1 this Matrix
	 * 
	 *  	If this matrix's dimention is 1, then,,
	 * 		 result is 1 element matrix;
	 *  	If this matrix's dimention is 2, then...
	 * 		if axis is 0, calcrate veatrical sum  array.
	 *  		else if axis is 1, calcrate horizonal sum array.
	 *   	If this matrix's dimention is 3 ,then...
 	 * 		if axis is 0, calcrate veatrical sum face.
	 *  		else if axis is 1, calcrate horizonal sum face.
	 * 		else if axis is 2, calcrate depth sum face. 
	 * ***************************************************************************************/

	Matrix<TYPE> sum(int axis)
	{
		assert(height != 0 && channel != 0);
		if ((width == 0 || height == 0) && channel == 1)
		{
			Matrix<TYPE> sum(1, 1, 1);
			for (int i = 0; i < (this->width * this->height * this->channel); i++)
			{
				sum.data[0] = sum.data[0] + this->data[i];
			}
			return sum;
		}
		else if (channel == 1)
		{
			if (axis == 0)
			//row direction
			{
				Matrix<TYPE> sum(1, this->height);
				for (int i = 0; i < this->width; i++)
				{
					for (int y = 0; y < this->height; y++)
					{
						sum.data[sum.calc_pos(0, y, 0)] = sum.data[sum.calc_pos(0, y, 0)] + this->data[calc_pos(i, y, 0)];
					}
				}
				return sum;
			}
			else if (axis == 1)
			//colmn direction
			{
				Matrix<TYPE> sum(this->width, 1);
				for (int i = 0; i < this->height; i++)
				{
					for (int x = 0; x < this->height; x++)
					{
						sum.data[sum.calc_pos(x, 0, 0)] = sum.data[sum.calc_pos(x, 0, 0)] + this->data[calc_pos(x, i, 0)];
					}
				}
				return sum;
			}
		}
		else if (1 < channel)
		{
			if (axis == 0)
			//row direction
			{
				Matrix<TYPE> sum(1, this->height, this->channel);
				for (int i = 0; i < this->width; i++)
				{
					for (int y = 0; y < this->height; y++)
					{
						for (int z = 0; z < this->channel; z++)
						{
							sum.data[sum.calc_pos(0, y, z)] = sum.data[sum.calc_pos(0, y, z)] + this->data[calc_pos(i, y, z)];
						}
					}
				}
				return sum;
			}

			else if (axis == 1)
			//column direction
			{
				Matrix<TYPE> sum(this->width, 1, this->channel);
				for (int i = 0; i < this->height; i++)
				{
					for (int x = 0; x < this->height; x++)
					{
						for (int z = 0; z < this->channel; z++)
						{
							sum.data[sum.calc_pos(x, 0, z)] = sum.data[sum.calc_pos(x, 0, z)] + this->data[calc_pos(x, i, z)];
						}
					}
				}
				return sum;
			}
			else if (axis == 2)
			//depth direction
			{
				Matrix<TYPE> sum(this->width, this->height, 1);

				for (int i = 0; i < this->channel; i++)
				{
					for (int x = 0; x < this->height; x++)
					{
						for (int y = 0; y < this->channel; y++)
						{
							sum.data[sum.calc_pos(x, y, 0)] = sum.data[sum.calc_pos(x, y, 0)] + this->data[calc_pos(x, y, i)];
						}
					}
				}
				return sum;
			}
		}
	}
	// Operator Over Rodes

	//assign
	Matrix<TYPE> operator<<(double x)
	{
		data[cnt] = x;
		cnt++;
		return *this;
	}

	//matrix vs matrix
	Matrix<TYPE> operator+(Matrix<TYPE> obj)
	{

		if (this->width == obj.width && this->height == obj.height && this->channel == obj.channel)
		//simple adder
		{
			assert(this->width == obj.width && this->height == obj.height && this->channel == obj.channel);
			Matrix<TYPE> result(this->width, this->height, this->channel);
			for (int i = 0; i < width * height * channel; i++)
			{
				result.data[i] = this->data[i] + obj.data[i];
			}
			return result;
		}
		//2d vs vector
		else if (this->width == obj.width && obj.height == 1 && obj.channel == 1)
		{
			Matrix<TYPE> result(this->width, this->height, this->channel);
			for (int c_index = 0; c_index < this->channel; c_index++)
			{
				for (int h_index = 0; h_index < this->height; h_index++)
				{
					for (int w_index = 0; w_index < this->width; w_index++)
					{
						result.data[this->calc_pos(w_index, h_index, c_index)] = this->data[this->calc_pos(w_index, h_index, c_index)] + obj.data[obj.calc_pos(w_index, 1, 1)];
					}
				}
			}
			return result;
		}
		else if (this->width == 1 && obj.height == this->height && obj.channel == 1)
		{
			Matrix<TYPE> result(this->width, this->height, this->channel);
			for (int c_index = 0; c_index < this->channel; c_index++)
			{
				for (int h_index = 0; h_index < this->height; h_index++)
				{
					for (int w_index = 0; w_index < this->width; w_index++)
					{
						result.data[this->calc_pos(w_index, h_index, c_index)] = this->data[this->calc_pos(w_index, h_index, c_index)] + obj.data[obj.calc_pos(1, h_index, 1)];
					}
				}
			}
			return result;
		}
		else if (this->width == obj.width && obj.height == 1 && obj.channel == 1)
		{
			Matrix<TYPE> result(this->width, this->height, this->channel);
			for (int c_index = 0; c_index < this->channel; c_index++)
			{
				for (int h_index = 0; h_index < this->height; h_index++)
				{
					for (int w_index = 0; w_index < this->width; w_index++)
					{
						result.data[this->calc_pos(w_index, h_index, c_index)] = this->data[this->calc_pos(w_index, h_index, c_index)] + obj.data[obj.calc_pos(1, 1, c_index)];
					}
				}
			}
			return result;
		}
		//3D vs 2D
		else if (this->width == obj.width && obj.height == this->height && obj.channel == 1)
		//Z axis face
		{
			Matrix<TYPE> result(this->width, this->height, this->channel);
			for (int c_index = 0; c_index < this->channel; c_index++)
			{
				for (int h_index = 0; h_index < this->height; h_index++)
				{
					for (int w_index = 0; w_index < this->width; w_index++)
					{
						result.data[this->calc_pos(w_index, h_index, c_index)] = this->data[this->calc_pos(w_index, h_index, c_index)] + obj.data[obj.calc_pos(w_index, h_index, 1)];
					}
				}
			}
			return result;
		}
		else if (this->width == obj.width && obj.height == 1 && obj.channel == this->channel)
		//Y axis face
		{
			Matrix<TYPE> result(this->width, this->height, this->channel);
			for (int c_index = 0; c_index < this->channel; c_index++)
			{
				for (int h_index = 0; h_index < this->height; h_index++)
				{
					for (int w_index = 0; w_index < this->width; w_index++)
					{
						result.data[this->calc_pos(w_index, h_index, c_index)] = this->data[this->calc_pos(w_index, h_index, c_index)] + obj.data[obj.calc_pos(w_index, 1, c_index)];
					}
				}
			}
			return result;
		}
		else if (this->width == obj.width && obj.height == 1 && obj.channel == this->channel)
		//Z axis face
		{
			Matrix<TYPE> result(this->width, this->height, this->channel);
			for (int c_index = 0; c_index < this->channel; c_index++)
			{
				for (int h_index = 0; h_index < this->height; h_index++)
				{
					for (int w_index = 0; w_index < this->width; w_index++)
					{
						result.data[this->calc_pos(w_index, h_index, c_index)] = this->data[this->calc_pos(w_index, h_index, c_index)] + obj.data[obj.calc_pos(1, h_index, c_index)];
					}
				}
			}
			return result;
		}
	}

	Matrix<TYPE> operator-(Matrix<TYPE> obj)
	{
		assert(this->width == obj.width && this->height == obj.height && this->channel == obj.channel);
		Matrix<TYPE> result(this->width, this->height, this->channel);
		for (int i = 0; i < width * height * channel; i++)
		{
			result.data[i] = this->data[i] - obj.data[i];
		}
		return result;
	}

	Matrix<TYPE> operator*(Matrix<TYPE> obj)
	{
		assert(this->width == obj.width && this->height == obj.height && this->channel == obj.channel);
		Matrix<TYPE> result(this->width, this->height, this->channel);
		for (int i = 0; i < width * height * channel; i++)
		{
			result.data[i] = this->data[i] * obj.data[i];
		}
		return result;
	}

	//matrix vs double
	Matrix<TYPE> operator+(double x)
	{
		Matrix<TYPE> result(this->width, this->height, this->channel);
		for (int i = 0; i < width * height * channel; i++)
		{
			result.data[i] = this->data[i] + x;
		}
		return result;
	}

	Matrix<TYPE> operator*(double x)
	{
		Matrix<TYPE> result(this->width, this->height, this->channel);
		for (int i = 0; i < width * height * channel; i++)
		{
			result.data[i] = this->data[i] * x;
		}
		return result;
	}

	Matrix<TYPE> operator/(double x)
	{
		Matrix<TYPE> result(this->width, this->height, this->channel);
		for (int i = 0; i < width * height * channel; i++)
		{
			result.data[i] = this->data[i] / x;
		}
		return result;
	}
	//clone
	Matrix<TYPE> clone(void)
	{
		assert(this->data != NULL);
		if (height == 0 || channel == 0)
		{
			Matrix<TYPE> cloned(this->width);
			for (int i = 0; i < width; i++)
			{
				cloned.data[i] = this->data[i];
			}
			return cloned;
		}
		else
		{
			Matrix<TYPE> cloned(this->width, this->height, this->channel);
			for (int i = 0; i < width * height * channel; i++)
			{
				cloned.data[i] = this->data[i];
			}
			return cloned;
		}
		//don't copy counter of <<
	}

	// overwrite by random parameter
	Matrix<TYPE> random(void)
	{
		assert(this->width > 0 && this->height > 0 && this->channel > 0);
		Matrix<TYPE> result(this->width, this->height, this->channel);
		result.shape();
		std::random_device seed_gen;
		std::default_random_engine engine(seed_gen());
		std::normal_distribution<double> dist(0.0, 1.0);

		for (int i; i < width * height * channel; i++)
		{
			result.data[i] = (double)dist(engine);
		}
		return result;
	}
};
} // namespace mat
#endif