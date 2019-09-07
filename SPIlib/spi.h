#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include "SPICallback.h"

namespace SPI
{

class Registers
{
	uint8_t SPIControlRegister;
	uint8_t SPIStatusRegister;
};

inline void enable() { SPCR |= (1<<SPE); }
inline void disable() { SPCR &= ~(1<<SPE); }
inline void setEnabled(bool enabled) { SPCR ^= (SPCR & (1<<SPE)) ^ (enabled<<SPE); }
inline bool enabled() { return (SPCR >> SPE) & 1; }

enum class DataOrder : uint8_t
{
	MSBFirst = 0,
	LSBFirst = (1<<DORD)
};
inline void setDataOrder(DataOrder order) { SPCR ^= (SPCR & (1<<DORD)) ^ static_cast<uint8_t>(order); }
inline DataOrder dataOrder() { return static_cast<DataOrder>(SPCR & (1<<DORD)); }

enum class Mode : uint8_t
{
	Slave = 0,
	Master = (1<<MSTR)
};
inline void setMode(Mode mode) { SPCR ^= (SPCR & (1<<MSTR)) ^ static_cast<uint8_t>(mode); }
inline Mode mode() { return static_cast<Mode>(SPCR & (1<<MSTR)); }

enum class IdleClockLevel : uint8_t
{
	Low = 0,
	High = (1<<CPOL)
};
enum class SamplingEdge : uint8_t
{
	Leading = 0,
	Trailing = (1<<CPHA)
};
inline void configureClock(IdleClockLevel idleLevel, SamplingEdge action) { SPCR ^= (SPCR & (1<<CPHA | 1<<CPOL)) ^ (static_cast<uint8_t>(idleLevel) | static_cast<uint8_t>(action)); }
inline IdleClockLevel idleClockLevel() { return static_cast<IdleClockLevel>(SPCR & (1<<CPOL)); }
inline SamplingEdge samplingEdge() { return static_cast<SamplingEdge>(SPCR & (1<<CPHA)); }

enum class PrescalerFactor : uint8_t
{
	DIV2	= 0b100,
	DIV4	= 0b000,
	DIV8	= 0b101,
	DIV16	= 0b001,
	DIV32	= 0b110,
	DIV64	= 0b010,
	DIV128	= 0b011
};
void setMasterClockFactor(PrescalerFactor factor);

inline void writeData(uint8_t data) { SPDR = data; }
inline uint8_t data() { return SPDR; }

inline bool hasPendingData() { return SPSR & (1<<SPIF); }

void setTransportCompleteCallback(SPICallback *cb);

inline void wait() { while(!(SPSR & (1<<SPIF))); }

inline uint8_t transceive(uint8_t data)
{
	// Load data into the buffer
	SPDR = data;
	
	wait();
	
	// Return received data
	return(SPDR);
}

template<typename SizeType = uint8_t>
void transceive(uint8_t *buffer, SizeType size)
{
	for (SizeType i = 0; i < size; ++i)
		buffer[i] = transceive(buffer[i]);
}

}

#endif /* SPI_H_ */
