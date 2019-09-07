#ifndef __ADCCALLBACK_H__
#define __ADCCALLBACK_H__

#include <stdint.h>

class ADCCallback
{
public:
	virtual void sample(uint16_t sample) = 0;
	
private:
};

#endif //__ADCCALLBACK_H__
