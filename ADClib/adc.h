#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include "ADCCallback.h"

namespace ADConverter
{

struct Registers
{
	uint8_t ADCControlAndStatusRegisterA;
	uint8_t ADCControlAndStatusRegisterB;
	uint8_t ADCMultiplexerSelectionRegister;
	uint8_t DigitalInputDisableRegister;
};

inline void enable() { ADCSRA |= (1<<ADEN); }
inline void disable() { ADCSRA &= ~(1<<ADEN); }
inline void setEnabled(bool enabled) { ADCSRA ^= (ADCSRA & (1<<ADEN)) ^ (enabled<<ADEN); }
inline bool enabled() { return (ADCSRA >> ADEN) & 1; }
	
inline void startConversion() { ADCSRA |= (1<<ADSC); }
inline bool conversionStarted() { return (ADCSRA >> ADSC) & 1; }
	
inline void enableAutoTrigger()  { ADCSRA |= (1<<ADATE);  }
inline void disableAutoTrigger() { ADCSRA &= ~(1<<ADATE); }
inline void setAutoTriggerEnabled(bool enabled) { ADCSRA ^= (ADCSRA & (1<<ADATE)) ^ (enabled<<ADATE); }
inline bool autoTriggerEnabled() { return (ADCSRA >> ADATE) & 1; }
	
inline bool interruptFlag() { return (ADCSRA >> ADIF) & 1; }
inline void clearInterruptFlag() { ADCSRA |= (1 << ADIF); } // cleared by writing 1 to it
	
inline void enableInterrupt() {  ADCSRA |= (1<<ADIE); }
inline void disableInterrupt() { ADCSRA &= ~(1<<ADIE); }
inline void setInterruptEnabled(bool enabled) { ADCSRA ^= (ADCSRA & (1<<ADIE)) ^ (enabled<<ADIE); }
inline bool interruptEnabled() { return (ADCSRA >> ADIE) & 1; }
	
// Conversion clock must be between 50kHz and 200kHz for 10-bit conversions
enum class PrescalerFactor : uint8_t
{
	DIV2	= 0b001,
	DIV4	= 0b010,
	DIV8	= 0b011,
	DIV16	= 0b100,
	DIV32	= 0b101,
	DIV64	= 0b110,
	DIV128	= 0b111
};
inline void setPrescalerFactor(PrescalerFactor factor) { ADCSRA ^= (ADCSRA & 0b0111) ^ static_cast<uint8_t>(factor); }
inline PrescalerFactor prescalerFactor() { return static_cast<PrescalerFactor>(ADCSRA & 0b0111); }
	
// Positive edges trigger conversions
enum class AutoTriggerSource : uint8_t
{
	FreeRunning						= 0b000,
	AnalogComparator				= 0b001,
	ExternalInterruptRequest0		= 0b010,
	TimerCounter0CompareMatchA		= 0b011,
	TimerCounter0Overflow			= 0b100,
	TimerCounter1CompareMatchB		= 0b101,
	TimerCounter1Overflow			= 0b110,
	TimerCounter1CaptureEvent		= 0b111,
};
inline void setAutoTriggerSource(AutoTriggerSource source) { ADCSRB ^= (ADCSRB & 0b0111) ^ static_cast<uint8_t>(source); }
inline AutoTriggerSource autoTriggerSource() { return static_cast<AutoTriggerSource>(ADCSRB & 0b0111); }

enum class SampleAlignment : uint8_t
{
	Right = 0,
	Left = (1<<ADLAR)
};
inline void setSampleAlignment(SampleAlignment side) { ADMUX ^= (ADMUX & (1<<ADLAR)) ^ static_cast<uint8_t>(side); }
inline SampleAlignment sampleAlignment() { return static_cast<SampleAlignment>(ADMUX & (1<<ADLAR)); }

enum class VoltageReference : uint8_t
{
	AREF		= 0x00,
	AVCC		= 0x40,
	Internal1v1 = 0xC0,
};
inline void setVoltageReference(VoltageReference reference) { ADMUX ^= (ADMUX & 0xC0) ^ static_cast<uint8_t>(reference); }
inline VoltageReference voltageReference() { return static_cast<VoltageReference>(ADMUX & 0xC0); }
inline bool isVoltageReferenceValid(uint8_t reference) { return reference == VoltageReference::AREF ||
	reference == VoltageReference::AVCC || reference == VoltageReference::Internal1v1; }
	
enum class InputChannel : uint8_t
{
	ADC0	 = 0b0000,
	ADC1	 = 0b0001,
	ADC2	 = 0b0010,
	ADC3	 = 0b0011,
	ADC4	 = 0b0100,
	ADC5	 = 0b0101,
	ADC6	 = 0b0110,
	ADC7	 = 0b0111,
	REF1v1	 = 0b1110,
	GND		 = 0b1111
};
inline void setInputChannel(InputChannel channel) { ADMUX ^= (ADMUX & 0x0F) ^ static_cast<uint8_t>(channel); }
inline InputChannel inputChannel() { return static_cast<InputChannel>(ADMUX & 0x0F); }
inline bool isInputChannelValid(uint8_t channel) { return channel <= 0b0111 || channel >= 0b1110; }

inline void waitForData() { while (conversionStarted()); }

void setSampleCallback(ADCCallback *cb);
}

#endif /* ADC_H_ */
