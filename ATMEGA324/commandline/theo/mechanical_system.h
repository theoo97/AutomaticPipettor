#define MAX_SPEED       1000
#define DELAY_CONST     2000

#define DIRECTION_LEFT  0
#define DIRECTION_RIGHT 1

typedef struct mechanical_system {
	int direction1, direction2;
	int speed1, speed2;
	int height1, height2;
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

	if (direction != DIRECTION_LEFT && direction != DIRECTION_RIGHT) {
		return;
	}

	ms->direction1 = direction;
}

void set_direction2(mechanical_system *ms, int direction) {
	if(!ms) {
		return;
	}

	if (direction != DIRECTION_LEFT && direction != DIRECTION_RIGHT) {
		return;
	}

	ms->direction1 = direction;
}

