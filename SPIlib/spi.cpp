#include "spi.h"

#include <avr/interrupt.h>

static SPICallback *callback = nullptr;
	
static void enableInterrupt() {  SPCR |= (1<<SPIE); }
static void disableInterrupt() { SPCR &= ~(1<<SPIE); }

void SPI::setTransportCompleteCallback(SPICallback *cb)
{
	if (cb == nullptr)
	{
		disableInterrupt();
		callback = cb;
	}
	else
	{
		callback = cb;
		enableInterrupt();
	}
}

ISR(SPI_STC_vect)
{
	if (callback)
		callback->read(SPI::data());
}


/*
SPI::SPI(SPI::Mode mode)
{
	if (mode == SPI::Mode::Master) {
		// Set MOSI and SCK as output
		DDRB |= (1<<DD5)|(1<<DD3);
		
		SPCR |= (1<<MSTR);
	}
	else {
		// Set MISO output, all others input
		DDRB = (1<<DD6);
	}
	
	// Enable SPI
	SPCR |= (1<<SPE);
}
*/

