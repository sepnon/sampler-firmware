#ifndef __SPICALLBACK_H__
#define __SPICALLBACK_H__

class SPICallback
{
public:
	virtual void read(uint8_t data) = 0;
	
	virtual ~SPICallback() = default;
};

#endif //__SPICALLBACK_H__
