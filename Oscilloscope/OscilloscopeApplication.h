#ifndef __OSCILLOSCOPEAPPLICATION_H__
#define __OSCILLOSCOPEAPPLICATION_H__

#include <ADCCallback.h>
#include <circular_buffer.h>
#include "AdcBuffer.h"

class OscilloscopeApplication // : private ADCCallback
{
public:
	OscilloscopeApplication();
	OscilloscopeApplication( const OscilloscopeApplication &c ) = delete;
	OscilloscopeApplication& operator=( const OscilloscopeApplication &c ) = delete;
	~OscilloscopeApplication();

	void startSampling();
	void stopSampling();
	void readSamples();
	void readStatus();
	
	void setInputChannel();
	void readInputChannel();
	
	void setVoltageReference();
	void readVoltageReference();
	
	void readByte();
	void incByte();
	
	void count();

	// virtual void sample(uint16_t sample) override final;	

private:
	uint8_t _byte = 0;
};

#endif //__OSCILLOSCOPEAPPLICATION_H__
