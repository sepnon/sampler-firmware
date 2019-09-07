#include <avr/io.h>
#include <avr/interrupt.h>

#include "spi.h"
#include "blink.h"
#include "OscilloscopeApplication.h"

enum Command : uint8_t
{
	START_SAMPLING = 1,
	STOP_SAMPLING = 2,
	READ_SAMPLES = 3,
	READ_STATUS = 4,
	COUNT = 10,
	INC_BYTE = 20,
	READ_BYTE = 21,
};

int main(void)
{
	CLKPR = (1<<CLKPCE);
	CLKPR = 0;
	
	DDRB = (1<<DDB4);
	
	SPI::enable();
	SPI::setMode(SPI::Mode::Slave);
	SPI::configureClock(SPI::IdleClockLevel::Low, SPI::SamplingEdge::Leading);
	
	sei();
	
	startBlink();
	
    OscilloscopeApplication app;
    while(1)
    {
	    switch(SPI::transceive(0))
	    {
		    case START_SAMPLING:
		    app.startSampling();
		    break;
		    
		    case STOP_SAMPLING:
		    app.stopSampling();
		    break;
		    
		    case READ_SAMPLES:
		    app.readSamples();
		    break;
		    
		    case READ_STATUS:
		    app.readStatus();
		    break;
			
			case INC_BYTE:
			app.incByte();
			break;
			
			case READ_BYTE:
			app.readByte();
			break;
		    
			case COUNT:
			app.count();
			break;
			
		    default:
		    break;
	    }
    }
}

