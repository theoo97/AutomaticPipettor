#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include "mechanical_system.h"

mechanical_system mechanical_systems[1];

void delay_ms(uint16_t count) {
	while(count--) {
    		_delay_ms(1);

	}
}

/*****************************************************************************
 ***************************** Bluetooth Stuff *******************************
 *****************************************************************************/

void bluetooth_init() {
        // Setează baud rate la 9600.
        UBRR0H = 0;
        UBRR0L = 103;

        // Porneste transmitatorul si receptorul.
        UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);

        // Formatul frame-ului: 8 biți de date, 1 bit de stop, fără paritate.
        UCSR0C &= ~(1 << USBS0);
        UCSR0C |= (2 << UCSZ00);
}

void bluetooth_send_char(char data) {
        // Asteapta pana cand buffer-ul e gol.
        while(!(UCSR0A & (1<<UDRE0)));

        // Pune datele in buffer; transmisia va porni automat in urma scrierii.
        UDR0 = data;
}

void bluetooth_send(char *buffer, int size) {
	int i;
	for (i = 0; i < size; i++) {
		bluetooth_send_char(buffer[i]);
	}
}

volatile char cmd[CMD_BUFSIZE];
volatile int cmd_i = 0;

void parse_and_execute_cmd() {
	char cmd_cpy[CMD_BUFSIZE];
	strcpy(cmd_cpy, (char *)cmd);

	// Execute command on a single mechanical system.
	ms_execute(&mechanical_systems[0], cmd_cpy);

	// Print executed command to terminal.
	/*
	bluetooth_send((char *)"Command: ", 9);
	bluetooth_send((char *)cmd, cmd_i + 1);
	bluetooth_send((char *)"\n", 1);
	*/
	// Print mechanical system state to terminal.
	char str[TO_STR_BUFSIZE];
	to_string(&mechanical_systems[0], str);
	bluetooth_send(str, strlen((const char *)str));
}

ISR(USART0_RX_vect) {
        char data = UDR0;

        cmd[cmd_i] = data;

	if (data == '\n') {
		cmd[cmd_i] = '\0';
		parse_and_execute_cmd();
		cmd_i = 0;
	}
	else {
		cmd_i++;
	}

}

int main() {
	// Pin iesire.
	DDRD |= (1 << PD7);

	int i;
	for (i = 0; i < 4; i++) {
		PORTD ^= (1 << PD7);
                delay_ms(50);
	}

	cli();
	bluetooth_init();
	sei();

	init_mechanical_systems(mechanical_systems, 1);
	run_mechanical_systems(mechanical_systems, 1);

	return 0;
}
