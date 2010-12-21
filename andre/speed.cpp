#include "speed.h"
#include "common.h"

Speed::Speed() {
	left = DEFAULT_LEFT;
	right = DEFAULT_RIGHT;
	time = 0;
}

int Speed::getLeft() {
	return left;
}

int Speed::getRight() {
	return right;
}

void Speed::run(Robot *robot, unsigned long delta) {
	bool *sensors = robot->getDistanceSensors();
	unsigned long maxTime = STOP_TIME*2*50/(DEFAULT_LEFT+DEFAULT_RIGHT);
	if(sensors[0]) {
		if(time+delta <= maxTime) {
			time += delta;
		} else {
			time = maxTime;
		}
	} else {
		if(time>=delta) {
			time -= delta;
		} else {
			time = 0;
		}
	}
	float factor = 1-(time/(float)maxTime);
	left = (int)(DEFAULT_LEFT*factor);
	right = (int)(DEFAULT_RIGHT*factor);
}
