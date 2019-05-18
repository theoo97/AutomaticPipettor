#ifndef MECHANICAL_SYSTEM_H
#define MECHANICAL_SYSTEM_H

#include <avr/io.h>
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

// Main program loop. Runs all the mechanical systems.
void run_mechanical_systems(mechanical_system *ms, size_t size) {
	// Main loop.
       	int clk = 0;
	while(1) {
		// Control each mechanical system, on eack clk step.
		for (unsigned int i = 0; i < size; i++) {
			if (ms[i].speed1 == 0) {
				ms[i].speed1 = DEFAULT_SPEED;
			}
			if (ms[i].speed2 == 0) {
				ms[i].speed2 = DEFAULT_SPEED;
			}

			// Control motor 1.

			// Control motor 2.
			int speed = ms[i].speed2 != 0 ? ms[i].speed2 : 500;
			const int delay_mot2 = (2 * MAX_SPEED) / speed;
			int state = clk % (4 * delay_mot2);
			if(ms[i].motor2_moving && ms[i].direction2 == DIRECTION_UP) {
				if (state == 0) {
					PORTA = (1 << PA7);
				} else if (state == delay_mot2) {
					PORTA = (1 << PA6);
				} else if (state == delay_mot2 * 2) {
					PORTA = (1 << PA5);
				} else if (state == delay_mot2 * 3) {
					PORTA = (1 << PA4);
				}
			}
			if(ms[i].motor2_moving && ms[i].direction2 == DIRECTION_DOWN) {
				if (state == 0) {
					PORTA = (1 << PA4);
				} else if (state == delay_mot2) {
					PORTA = (1 << PA5);
				} else if (state == delay_mot2 * 2) {
					PORTA = (1 << PA6);
				} else if (state == delay_mot2 * 3) {
					PORTA = (1 << PA7);
				}
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
