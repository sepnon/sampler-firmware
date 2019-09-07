#include "adc.h"

#include <avr/interrupt.h>

static ADCCallback *callback = nullptr;

void ADConverter::setSampleCallback(ADCCallback *cb)
{
	callback = cb;
}

/*
ISR(ADC_vect)
{
	if (callback != nullptr)
	{
		// ADCL must be read first
		const uint8_t low = ADCL;
		uint16_t sample = ADCH;
		sample <<= 8;
		sample |= low;
		callback->sample(sample);
	}
}
*/
