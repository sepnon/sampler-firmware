#include <avr/interrupt.h>

ISR(TIMER0_OVF_vect)
{
	PORTC ^= (1<<DDC5);
}

void startBlink()
{
	DDRC |= (1<<DDC5);
	PORTC &= ~(1<<DDC5);

	TCCR0B |= (1 << CS02)|(1 << CS00); // prescaler = clkio/1024
	
	TIMSK0 |= (1 << TOIE0); // enable overflow interrupt
}

void stopBlink()
{
	TCCR0B &= ~(1 << CS02)|(1 << CS01)|(1 << CS00);
}
