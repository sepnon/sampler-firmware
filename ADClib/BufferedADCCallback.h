#ifndef __BUFFEREDADCCALLBACK_H__
#define __BUFFEREDADCCALLBACK_H__

#include "ADCCallback.h"
#include <circular_buffer.h>

template<uint16_t size>
class BufferedADCCallback : public ADCCallback
{
public:
	void take(uint16_t *buffer, uint16_t bufferSize);
	
	void sample(uint16_t sample) override;

private:
	circular_buffer<uint16_t, size> _buffer;
};

#endif //__BUFFEREDADCCALLBACK_H__
