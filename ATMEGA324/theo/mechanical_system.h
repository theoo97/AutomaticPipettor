#ifndef MECHANICAL_SYSTEM_H
#define MECHANICAL_SYSTEM_H

#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>

#define MAXCLK		20000

#define MAX_SPEED       1000
#define DEFAULT_SPEED	500

#define DIRECTION_UP  0
#define DIRECTION_DOWN 1
#define TO_STR_BUFSIZE 500

#define CMD_BUFSIZE 100
#define MOTOR1  1
#define MOTOR2  2
#define MIN_HEIGHT1 60
#define MAX_HEIGHT1 250

#define MIN_HEIGHT2 30
#define MAX_HEIGHT2 80

typedef struct mechanical_system {
	int direction1, direction2;
	int speed1, speed2;
	int height1, height2;
	int motor1_moving, motor2_moving;
} mechanical_system;

void set_speed1(mechanical_system *ms, int speed) {
	if(!ms) {
		return;
	}

	ms->speed1 = speed;
}
void set_speed2(mechanical_system *ms, int speed) {
	if(!ms) {
		return;
	}

	ms->speed2 = speed;
}

void set_direction1(mechanical_system *ms, int direction) {
	if(!ms) {
		return;
	}

	if (direction != DIRECTION_UP && direction != DIRECTION_DOWN) {
		return;
	}

	ms->direction1 = direction;
}

void set_direction2(mechanical_system *ms, int direction) {
	if(!ms) {
		return;
	}

	if (direction != DIRECTION_UP && direction != DIRECTION_DOWN) {
		return;
	}

	ms->direction1 = direction;
}

void ms_execute(mechanical_system *ms, char *cmd) {
	char *cmd_token, *token;
	int device_number;

	if (!ms) {
		return;
	}

	cmd_token = strtok((char*)cmd, " ");
	if (!cmd_token) {
		return;
	}

	token = strtok(NULL, " ");
	if (!token) {
		return;
	}

	device_number = atoi(token);

	if (strcmp(cmd_token, "MOVEUP") == 0) {
		if (device_number == MOTOR1) {
			ms->direction1 = DIRECTION_UP;
			ms->motor1_moving = 1;
		} else if (device_number == MOTOR2) {
			ms->direction2 = DIRECTION_UP;
			ms->motor2_moving = 1;
		}
	}
	else if (strcmp(cmd_token, "MOVEDOWN") == 0) {
		if (device_number == MOTOR1) {
			ms->direction1 = DIRECTION_DOWN;
			ms->motor1_moving = 1;
		} else if (device_number == MOTOR2) {
			ms->direction2 = DIRECTION_DOWN;
			ms->motor2_moving = 1;
		}
	}
	else if (strcmp(cmd_token, "STOPMOVE") == 0) {
		if (device_number == MOTOR1) {
			ms->motor1_moving = 0;
		} else if (device_number == MOTOR2) {
			ms->motor2_moving = 0;
		}
	}
	else if (strcmp(cmd_token, "SETSPEED") == 0) {
		token = strtok(NULL, " ");
		if (!token) {
			return;
		}

		int speed = atoi(token);

		if (device_number == MOTOR1) {
			ms->speed1 = speed;
		}
		else if (device_number == MOTOR2) {
			ms->speed2 = speed;
		}
	}
}

void to_string(mechanical_system *ms, char *str) {
	if (!ms) {
		return;
	}

	str[0] = '\0';
	sprintf((char *)str,
		"|=========================|\n"
		"| Mechanical System state  \n"
		"|=========================|\n"
		"| Dir1: %d\n"
		"| Speed1: %d\n"
		"| Height1: %d\n"
		"| Moving1: %d\n"
		"|\n"
		"| Dir2: %d\n"
		"| Speed2: %d\n"
		"| Height2: %d\n"
		"| Moving2: %d\n"
		"==========================|\n\n\n",
		ms->direction1,
		ms->speed1,
		ms->height1,
		ms->motor1_moving,
		ms->direction2,
		ms->speed2,
		ms->height2,
		ms->motor2_moving);
}

struct settings_t
{
	int height2;
};

void init_mechanical_systems(mechanical_system *ms, size_t size) {
	struct settings_t settings;

	// Pin iesire.
        DDRA |= (1 << PA7) | (1 << PA6) | (1 << PA5) | (1 << PA4);
        DDRC |= (1 << PC7) | (1 << PC6) | (1 << PC5) | (1 << PC4);


	// Pin iesire (trig).
	DDRB |= (1 << PB0);

	// Pin intrare (echo).
	DDRB &= ~(1 << PB1);
	PORTB |= (1 << PB1);

	// Write setting to permanent memory.
//	struct settings_t settings_write;
//	settings_write.height1 = 50;
//	eeprom_write_block((const void*)&settings_write, (void*)0, sizeof(settings_t));

	// Read settings from permanent memory.
   	eeprom_read_block((void*)&settings, (void*)0, sizeof(settings_t));

	ms[0].height2 = settings.height2;
}

void control_motor1(mechanical_system *ms, int clk) {
	if (!ms) {
		return;
	}

	if (ms->speed1 == 0) {
		ms->speed1 = DEFAULT_SPEED;
	}

	int speed = ms->speed1;
	const int delay_mot1 = (2 * MAX_SPEED) / speed;
	int state = clk % (4 * delay_mot1);

	if(ms->motor1_moving &&
	ms->direction1 == DIRECTION_UP &&
	ms->height1 < MAX_HEIGHT1) {
		if (state == 0) {
			PORTA = (1 << PA7);
		} else if (state == delay_mot1) {
			PORTA = (1 << PA6);
		} else if (state == delay_mot1 * 2) {
			PORTA = (1 << PA5);
		} else if (state == delay_mot1 * 3) {
			PORTA = (1 << PA4);
		}
	}
	if(ms->motor1_moving &&
	ms->direction1 == DIRECTION_DOWN &&
	ms->height1 > MIN_HEIGHT1) {
		if (state == 0) {
			PORTA = (1 << PA4);
		} else if (state == delay_mot1) {
			PORTA = (1 << PA5);
		} else if (state == delay_mot1 * 2) {
			PORTA = (1 << PA6);
		} else if (state == delay_mot1 * 3) {
			PORTA = (1 << PA7);
		}
	}
}

void control_motor2(mechanical_system *ms, int clk) {
	if (!ms) {
		return;
	}

	if (ms->speed2 == 0) {
		ms->speed2 = DEFAULT_SPEED;
	}

	int speed = ms->speed2;
	const int delay_mot2 = (2 * MAX_SPEED) / speed;
	int state = clk % (4 * delay_mot2);

	if(ms->motor2_moving &&
	ms->direction2 == DIRECTION_UP &&
	ms->height2 < MAX_HEIGHT2) {
		if (state == 0) {
			PORTC = (1 << PC3);
		} else if (state == delay_mot2) {
			PORTC = (1 << PC2);
		} else if (state == delay_mot2 * 2) {
			PORTC = (1 << PC1);
		} else if (state == delay_mot2 * 3) {
			PORTC = (1 << PC0);
		}
	}
	if(ms->motor2_moving &&
	ms->direction2 == DIRECTION_DOWN &&
	ms->height2 > MIN_HEIGHT2) {
		if (state == 0) {
			PORTC = (1 << PC0);
		} else if (state == delay_mot2) {
			PORTC = (1 << PC1);
		} else if (state == delay_mot2 * 2) {
			PORTC = (1 << PC2);
		} else if (state == delay_mot2 * 3) {
			PORTC = (1 << PC3);
		}
	}
}

void read_height1(mechanical_system *ms, int clk) {
	// Send trig pulse.
	PORTB |= (1 << PB0);
	_delay_us(10);
	PORTB &= ~(1 << PB0);

	int time = 0;

	// Wait pulse.
	while(!(PINB & (1 << PB1))) {
		time++;
		_delay_us(1);

		if (time > 10000) {
			break;
		}
	}

	// Count sonic wave time.
	time = 0;
	while(PINB & (1 << PB1)) {
		time++;
		_delay_us(1);

		if (time > 10000) {
			break;
		}
	}

	if (time < 0 ) {
		ms->height1 = 0;
	} else {
		ms->height1 = ((time / 20) * 34 ) / 10;
	}
}

// Main program loop. Runs all the mechanical systems.
void run_mechanical_systems(mechanical_system *ms, size_t size) {
	// Main loop.
       	int clk = 0;
	while(1) {
		// Control each mechanical system, on eack clk step.
		for (unsigned int i = 0; i < size; i++) {
			control_motor1(&ms[i], clk);
			control_motor2(&ms[i], clk);

			// Measure distance every 200ms.
			if (clk % 100 == 0) {
				read_height1(&ms[i], clk);
			}
		}
		clk++;
		if (clk > MAXCLK) {
			clk = 0;
		}

		_delay_ms(1);
	}
}

#endif
