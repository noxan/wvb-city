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

void Speed::run(Robot *robot, unsigned long delta, bool crossroad) {
	bool *sensors = robot->getDistanceSensors();
/*	if(crossroad) {
		if(sensors[0] || sensors[1]) {
			left = 0;
			right = 0;
		} else {
			left = DEFAULT_LEFT;
			right = DEFAULT_RIGHT;
		}
	} else {*/
		unsigned long maxTime = STOP_TIME*2*50/(DEFAULT_LEFT+DEFAULT_RIGHT);
		if(sensors[0] || (sensors[1] && crossroad)) {
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
		if(!crossroad && factor == 0) { // Hupe
			robot->play("C");
		}
//	}
/*	if(sensors[0] || (sensors[1] && crossroad)) {
		left = 0;
		right = 0;
	} else {
		left = DEFAULT_LEFT;
		right = DEFAULT_RIGHT;
	}*/
}
