#include "OscilloscopeApplication.h"
#include "adc.h"
#include "spi.h"

#include <avr/interrupt.h>

// Definition of the error function to call if the constructor goes bonkers
// extern "C" void __cxa_pure_virtual() { while (1); }
		
static constexpr uint8_t ReadBufferSize = 16;
static AdcBuffer<16, ReadBufferSize> _buffer;

ISR(ADC_vect)
{
		// ADCL must be read first
		const uint8_t low = ADCL;
		uint16_t sample = ADCH;
		sample <<= 8;
		sample |= low;
		if (!_buffer.full())
			_buffer.push(sample);
}

 OscilloscopeApplication::OscilloscopeApplication()
{
	ADConverter::setVoltageReference(ADConverter::VoltageReference::AVCC);
	ADConverter::setInputChannel(ADConverter::InputChannel::REF1v1);
	ADConverter::setAutoTriggerSource(ADConverter::AutoTriggerSource::FreeRunning);
	ADConverter::enableAutoTrigger();
	// ADConverter::setSampleCallback(this);
	ADConverter::enableInterrupt();
	ADConverter::setPrescalerFactor(ADConverter::PrescalerFactor::DIV64);
}

 OscilloscopeApplication::~OscilloscopeApplication() = default;

void OscilloscopeApplication::startSampling()
{
	ADConverter::enable();
	ADConverter::startConversion();
}

void OscilloscopeApplication::stopSampling()
{
	ADConverter::disable();
	_buffer.clear();
}

void OscilloscopeApplication::readSamples()
{
	uint8_t rcv = 3;
	do
	{
		if (_buffer.empty())
			rcv &= SPI::transceive(0);
		else
		{
			rcv &= SPI::transceive(ReadBufferSize);
			
			const uint16_t *nextBlock = _buffer.readNextBlock();
			for (int i = 0; i < ReadBufferSize; ++i)
			{
				rcv &= SPI::transceive(nextBlock[i] >> 8);
				rcv &= SPI::transceive(nextBlock[i] & 0xFF);
			}
		}
	}
	while (rcv == 3);
}

void OscilloscopeApplication::readStatus()
{
	SPI::transceive(ADConverter::conversionStarted());
}

void OscilloscopeApplication::readByte()
{
	SPI::transceive(_byte);
}

void OscilloscopeApplication::incByte()
{
    _byte += 1;
}

void OscilloscopeApplication::count()
{
	uint8_t i = 0;
	do {
		i = SPI::transceive(i+5);
	} while (i != 0);
}

void OscilloscopeApplication::setInputChannel()
{
	auto newChannel = SPI::transceive(0);
	if (ADConverter::isInputChannelValid(newChannel))
		ADConverter::setInputChannel(static_cast<ADConverter::InputChannel>(newChannel));
}

void OscilloscopeApplication::readInputChannel()
{
	SPI::transceive(ADConverter::inputChannel());
}

void OscilloscopeApplication::setVoltageReference()
{
	auto newVoltageReference = SPI::transceive(0);
	if (ADConverter::isVoltageReferenceValid(newVoltageReference))
		ADConverter::setVoltageReference(static_cast<ADConverter::VoltageReference>(newVoltageReference));
}

void OscilloscopeApplication::readVoltageReference()
{
	SPI::transceive(ADConverter::voltageReference());
}

/*
void OscilloscopeApplication::sample(uint16_t sample)
{
	if (!_buffer.full())
		_buffer.push(sample);
}
*/
