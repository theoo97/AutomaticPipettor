#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>


void delay_ms(uint16_t count) {
	while(count--) {
    		_delay_ms(1);

	}
}


int main() {
	// Pin iesire.
	DDRD |= (1 << PD7);

	// Pin iesire.
	DDRA |= (1 << PA7) | (1 << PA6) | (1 << PA5) | (1 << PA4);

	int i;

	for (i = 0; i < 4; i++) {
		PORTD ^= (1 << PD7);
                delay_ms(50);
	}


	PORTA |= (1 << PA1);

	i = 0;
	while(1) {
		// Aprindere led.
		//PORTD ^= (1 << PD7);


		if (!(PINB &(1 << PB2)))
                {
			PORTD ^= (1 << PD7);
			delay_ms(200);
		}


		PORTA ^= (1 << PA0);
		delay_ms(40);

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
