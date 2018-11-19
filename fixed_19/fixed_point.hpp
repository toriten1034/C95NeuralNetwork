#ifndef INCLUDE_FIX18
#define INCLUDE_FIX18
#include <cmath>
#include <iostream>
typedef unsigned int uint32;
//frac part [0:7]
//integer part [8:16]
// flag part[17]

class Fix18
{
      public:
	uint32 value;
	//constructor
	Fix18()
	{
		this->value = 0;
	}

	Fix18(double x)
	{
		double u = std::abs(x);
		int intger_part = (int)u;
		uint32 frac_part = (int)((u - (double)intger_part) * 256);
		uint32 tmp = ((intger_part << 8) + frac_part); // & 0x3FFFF;

		int flag = 0;
		if (0 > x)
		{
			tmp = (~tmp) + 1;
			flag = 1 << 17;
		}
		this->value = tmp | flag;
	}

	Fix18(int x)
	{
		this->value = x;
	}

	//convert to double value
	double to_double(void)
	{
		double tmp = 0;
		int signed_value = this->value;
		if (this->value >> 17)
		{
			signed_value = ~(this->value);
		}

		uint32 intger_part = ((signed_value) >> 8) & 0x1FF;
		uint32 frac_part = (double)(signed_value & 0xFF);

		tmp = intger_part + ((double)frac_part / 256.0);

		if (this->value >> 17)
		{
			tmp = -tmp;
		}
		return tmp;
	}
	void operator=(double x)
	{
		double u = std::abs(x);
		int intger_part = (int)u;
		uint32 frac_part = (int)((u - (double)intger_part) * 256);
		uint32 tmp = ((intger_part << 8) + frac_part); // & 0x3FFFF;

		int flag = 0;
		if (0 > x)
		{
			tmp = (~tmp) + 1;
			flag = 1 << 17;
		}

		this->value = tmp | flag;
	}
	//fix19_vs_fix19
	void operator=(Fix18 obj)
	{
		this->value = obj.value;
	}

	Fix18 operator+(Fix18 obj)
	{
		uint32 tmp = (this->value & 0x3FFFF) + (obj.value & 0x3FFFF);
		return Fix18((int)(tmp & 0x3FFFF));
	}

	Fix18 operator-(Fix18 obj)
	{

		uint32 tmp = (this->value & 0x3FFFF) + ((~obj.value + 1) & 0x3FFFF);
		return Fix18((int)(tmp & 0x3FFFF));
	}

	Fix18 operator*(Fix18 obj)
	{
		uint32 a = (this->value < 0) ? (this->value | 0xFFE000000) : this->value;
		uint32 b = (obj.value < 0) ? (obj.value | 0xFFE000000) : obj.value;

		uint32 tmp = a * b;
		return Fix18((int)((tmp >> 8) & 0x7FFFF));
	}

	int operator>(Fix18 obj)
	{
		if (this->to_double() > obj.to_double())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int operator<(Fix18 obj)
	{
		if (this->to_double() < obj.to_double())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	//cout
	friend std::ostream &operator<<(std::ostream &os, Fix18 &obj)
	{
		os << obj.to_double();
		return os;
	}

      private:
	uint32 to_minus(uint32 x)
	{
		return x;
	}
};

#endif