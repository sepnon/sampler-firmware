#include "BufferedADCCallback.h"

template<uint16_t size>
void BufferedADCCallback<size>::sample(uint16_t sample)
{
	_buffer.push(sample);
}
