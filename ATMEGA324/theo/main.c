#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

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




/*****************************************************************************
 ***************************** Bluetooth Stuff *******************************
 *****************************************************************************/

#define CMD_BUFSIZE 100
#define MOTOR1	1
#define MOTOR2	2

void USART0_init() {
        // Setează baud rate la 9600.
        UBRR0H = 0;
        UBRR0L = 103;

        // Porneste transmitatorul si receptorul.
        UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);

        // Formatul frame-ului: 8 biți de date, 1 bit de stop, fără paritate.
        UCSR0C &= ~(1 << USBS0);
        UCSR0C |= (2 << UCSZ00);
}

void USART0_transmit(unsigned char data) {
        // Asteapta pana cand buffer-ul e gol.
        while(!(UCSR0A & (1<<UDRE0)));

        // Pune datele in buffer; transmisia va porni automat in urma scrierii.
        UDR0 = data;
}

volatile char cmd[CMD_BUFSIZE];
volatile int cmd_i = 0;

void parse_and_execute_cmd() {
	int device_number;
	char *cmd_token, *token;

	int index = 0;

	cmd_token = strtok((char*)cmd, " ");
	if (!cmd_token) {
		return;
	}

	token = strtok(NULL, " ");
	if (!token) {
		return;
	}

	device_number = atoi(token);

	if (strcmp(token, "MOVEUP") == 0) {
		if (device_number == MOTOR1) {
			mechanical_systems[index].direction1 = DIRECTION_UP;
                        mechanical_systems[index].motor1_moving = 1;
		} else if (device_number == MOTOR2) {
			mechanical_systems[index].direction2 = DIRECTION_UP;
			mechanical_systems[index].motor2_moving = 1;
		}
	}
	else if (strcmp(token, "MOVEDOWN") == 0) {
		if (device_number == MOTOR1) {
			mechanical_systems[index].direction1 = DIRECTION_DOWN;
			mechanical_systems[index].motor1_moving = 1;
		} else if (device_number == MOTOR2) {
			mechanical_systems[index].direction2 = DIRECTION_DOWN;
			mechanical_systems[index].motor2_moving = 1;
		}
	}
	else if (strcmp(token, "STOPMOVE") == 0) {
		if (device_number == MOTOR1) {
			mechanical_systems[index].motor1_moving = 0;
		} else if (device_number == MOTOR2) {
			mechanical_systems[index].motor2_moving = 0;
		}
	}
	else if (strcmp(token, "SETSPEED") == 0) {
		token = strtok(NULL, " ");
		if (!token) {
			return;
		}

		int speed = atoi(token);

		if (device_number == MOTOR1) {
			mechanical_systems[index].speed1 = speed;
		}
		else if (device_number == MOTOR2) {
			mechanical_systems[index].speed2 = speed;
		}
        }
}

ISR(USART0_RX_vect) {
        char data = UDR0;
        USART0_transmit(data);

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

	// Pin iesire.
	DDRA |= (1 << PA7) | (1 << PA6) | (1 << PA5) | (1 << PA4);

	int i;

	for (i = 0; i < 4; i++) {
		PORTD ^= (1 << PD7);
                delay_ms(50);
	}


	cli();
	USART0_init();
	sei();


	PORTD ^= (1 << PD7);


	PORTA |= (1 << PA1);

	i = 0;
	while(1) {
		// Aprindere led.
		//PORTD ^= (1 << PD7);


		if (!(PINB &(1 << PB2)))
                {
			PORTD ^= (1 << PD7);
			PORTA ^= (1 << PA1);

			USART0_transmit('A' + i);
			i = (i + 1) % 26;

			delay_ms(200);
		}


		PORTA ^= (1 << PA0);
		delay_ms(20);

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
