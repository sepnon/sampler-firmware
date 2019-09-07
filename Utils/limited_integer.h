#ifndef __LIMITED_INTEGER_H__
#define __LIMITED_INTEGER_H__

#include <stdint.h>

template <typename Integer, Integer limit>
class limited_integer
{
public:
	limited_integer(Integer i)
	{
		_data[_idx] = i;
	}
	
	void operator+=(Integer i)
	{
		Integer result = _data[_idx];
		result += i;
		result %= limit;
		_data[1 - _idx] = result;
		_idx = 1 - _idx;
	}
	
	operator Integer() const
	{
		return _data[_idx];
	}
	
private:
	uint8_t _idx = 0;
	Integer _data[2];
};


#endif //__LIMITED_INTEGER_H__
