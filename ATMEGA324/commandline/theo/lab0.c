#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "mechanical_system.h"

int16_t speed = 1000;

mechanical_system mechanical_systems[1];

void delay_ms(uint16_t count) {
	while(count--) {
    		_delay_ms(1);

	}
}

int get_delay_from_speed(int speed) {
	return DELAY_CONST / speed;
}

int main() {
	// Pin iesire.
	DDRD |= (1 << PD7);

	// Pin iesire.
	DDRA |= (1 << PA7) | (1 << PA6) | (1 << PA5) | (1 << PA4);


	while(1) {
		// Aprindere led.
		PORTD ^= (1 << PD7);
		delay_ms(1000);
	}
	/*
	while(1) {
		int16_t delay_step = get_delay_from_speed(speed);

		if (!(PINB &(1 << PB2)))
		{
			PORTA = (1 << PA7);
			delay_ms(delay_step);

			PORTA = (1 << PA6);
			delay_ms(delay_step);

			PORTA = (1 << PA5);
			delay_ms(delay_step);

			PORTA = (1 << PA4);
			delay_ms(delay_step);
		}
	}
	*/
	return 0;
}
