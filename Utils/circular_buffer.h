#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#include <stdint.h>
#include <string.h>
#include "limited_integer.h"

template<typename ContentType, uint16_t maxSize>
class circular_buffer
{
public:
	circular_buffer() = default;
	circular_buffer(const circular_buffer &other) = delete;
	
	void push(ContentType t)
	{
		_data[_writeIdx] = t;
		_writeIdx += 1;
		_virtWriteIdx += 1;
	}
	
	ContentType take()
	{
		auto value = _data[_readIdx];
		
		_readIdx += 1;
		_virtReadIdx += 1;
		
		return value;
	}
	
	void take(ContentType *destination, uint16_t count)
	{
		const auto availableOnSingleCopy = maxSize - _readIdx;
		if (availableOnSingleCopy >= count) {
			memcpy(destination, &_data[_readIdx], count*sizeof(ContentType));
		}
		else {
			memcpy(destination, &_data[_readIdx], availableOnSingleCopy * sizeof(ContentType));
			memcpy(destination + availableOnSingleCopy, _data, (count - availableOnSingleCopy) * sizeof(ContentType));
		}
		_readIdx += count;
		_virtReadIdx += count;
	}
	
	void clear()
	{
		_readIdx = 0;
		_writeIdx = _readIdx;
		_virtReadIdx = 0;
		_virtWriteIdx = _virtReadIdx;
	}
	
	uint16_t size() const
	{
		const uint16_t writeIdx = _virtWriteIdx;
		const uint16_t readIdx = _virtReadIdx;
		return writeIdx >= readIdx ? writeIdx - readIdx : maxSize - readIdx + writeIdx + 1;
	}
	bool full() const { return size() == maxSize; }
	bool empty() const { return size() == 0; }
	constexpr uint16_t maxsize() const { return maxSize; }
	
private:
	ContentType _data[maxSize];
	
	limited_integer<uint16_t, maxSize> _readIdx = 0;
	limited_integer<uint16_t, maxSize> _writeIdx = _readIdx;
	
	// Virtual coordinates appear to lose 1 index on a buffer with size _maxSize + 1
	limited_integer<uint16_t, maxSize + 1> _virtReadIdx = 0;
	limited_integer<uint16_t, maxSize + 1> _virtWriteIdx = 0;
};

#endif //__CIRCULAR_BUFFER_H__
