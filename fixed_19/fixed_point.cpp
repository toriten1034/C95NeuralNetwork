#include <iostream>
#include <cmath>
typedef short int16;
typedef unsigned short uint16;

// date structure XX.XX
class Fix16
{
  public:
	int16 value;
	//constructor
	Fix16()
	{
		this->value = 0;
	}

	Fix16(int16 x)
	{
		this->value = x;
	}

	Fix16(double x)
	{
		int16 intger_part = (int)x;
		int16 frac_part = (int)((x - (double)intger_part) * 256);
		this->value = (intger_part << 8) + frac_part;
	}

	//substitute double value
	int16 operator=(double val)
	{
		int16 intger_part = (int)val;
		int16 frac_part = (int)((val - (double)intger_part) * 256);
		this->value = (intger_part << 8) + frac_part;
	}
	//subsutitute int16 value
	int16 operator=(int16 val)
	{
		this->value = val;
	}

	/**************Fix16 vs Fix16*******************/
	//subsutitute int16 value

	int16 operator=(Fix16 obj)
	{
		this->value = obj.value;
	}
	Fix16 operator+(Fix16 obj)
	{
		int16 tmp = this->value + obj.value;
		return Fix16(tmp);
	}
	Fix16 operator-(Fix16 obj)
	{
		int16 tmp = this->value - obj.value;
		return Fix16(tmp);
	}
	Fix16 operator*(Fix16 obj)
	{
		int tmp = (int)this->value * (int)obj.value;
		int16 result = tmp >> 8;
		return Fix16(result);
	}
	Fix16 operator/(Fix16 obj)
	{
		int tmp = ((int)this->value << 8) / ((int)obj.value);
		return Fix16((int16)(tmp));
	}
	//convert to double value
	double to_double(void)
	{
		double tmp = 0;
		tmp += (this->value) >> 8;
		tmp += (double)(this->value & 0x00FF) / 256.0;
		return tmp;
	}
};

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
		std::cout << std::hex << (this->value & 0x3FFFF) << std::endl;
		std::cout << std::hex << (obj.value & 0x3FFFF) << std::endl;

		uint32 tmp = (this->value & 0x3FFFF) + ((~obj.value + 1) & 0x3FFFF);
		return Fix18((int)(tmp & 0x3FFFF));
	}

	Fix18 operator*(Fix18 obj)
	{
		uint32 a = (this->value < 0) ? (this->value | 0xFFE000000) : this->value;
		uint32 b = (obj.value < 0) ? (obj.value | 0xFFE000000) : obj.value;

		uint32 tmp = a * b;
		return Fix18((int)(((tmp >> 8) & 0x7FFFF) & ~(1 << 18)));
	}

  private:
	uint32 to_minus(uint32 x)
	{
		return x;
	}
};

int main(void)
{
	Fix18 a(1.5);
	Fix18 b(-2.75);
	Fix18 c = a + b;
	std::cout << "a is " << a.to_double() << std::endl;
	std::cout << "b is " << b.to_double() << std::endl;
	std::cout << "c is " << c.to_double() << "\n"
			  << std::endl;

	a = -1.5;
	b = -0.5;
	c = a - b;
	std::cout << "a is " << a.to_double() << std::endl;
	std::cout << "b is " << b.to_double() << std::endl;
	std::cout << "c is " << c.to_double() << "\n"
			  << std::endl;

	a = -0.5;
	b = -2;
	c = a * b;
	std::cout << "a is " << a.to_double() << std::endl;
	std::cout << "b is " << b.to_double() << std::endl;
	std::cout << "c is " << c.to_double() << std::endl;
}