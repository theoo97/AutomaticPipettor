#define MAX_SPEED       1000
#define DELAY_CONST     2000

#define DIRECTION_UP  0
#define DIRECTION_DOWN 1

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

