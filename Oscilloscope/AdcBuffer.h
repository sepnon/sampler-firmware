#ifndef __ADCBUFFER_H__
#define __ADCBUFFER_H__

#include <stdint.h>

template <unsigned int block_size, unsigned int block_count>
class AdcBuffer
{
public:
	AdcBuffer() = default;
	AdcBuffer( const AdcBuffer &c ) = delete;
	AdcBuffer& operator=( const AdcBuffer &c ) = delete;
	~AdcBuffer() = default;
	
	uint16_t *readNextBlock()
	{
		_readBlock = (_readBlock + 1) % block_count;
		return _data[_readBlock];
	}
	
	void push(uint16_t data)
	{
		_data[_writeBlock][_writeOffset] = data;
		if (_writeOffset == (block_size - 1))
		{
			_writeOffset = 0;
			_writeBlock = (_writeBlock + 1) % block_count;
		}
		else
			_writeOffset += 1;
	}
	
	bool empty() const { return (_readBlock + 1) % block_count == _writeBlock; }
	
	bool full() const { return _writeBlock == _readBlock; }
	
	void clear()
	{
		_writeBlock = 0;
		_writeOffset = 0;
		_readBlock = block_count - 1;
	}
	
private:
	uint16_t _data[block_count][block_size];
	
	volatile uint8_t _writeBlock = 0;
	volatile uint8_t _writeOffset = 0;
	
	uint8_t _readBlock = block_count - 1;
};

#endif //__ADCBUFFER_H__
