#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>



#define MAXCLK 20000

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

	int clk = 0;
	while(1) {

		int16_t delay_step = 2; //get_delay_from_speed(speed);

		int state = clk % 8;

                if (!(PINB &(1 << PB2)))
                {
			switch(state) {
				case 0:
					PORTA = (1 << PA7);
					break;
				case 2:
					PORTA = (1 << PA6);
					break;
				case 4:
					PORTA = (1 << PA5);
					break;
				case 6:
					PORTA = (1 << PA4);
					break;
			}
                }

		clk++;
		if (clk > MAXCLK) {
			clk = 0;
		}

		_delay_ms(1);
	}

	return 0;
}
