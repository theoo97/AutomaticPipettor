#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

int main() {
	// Pin iesire.
	DDRD |= (1 << PD7);

	while(1) {
		PORTD ^= (1 << PD7);
                _delay_ms(500);
	}


	return 0;
}
