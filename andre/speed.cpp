#include "speed.h"
#include "common.h"

Speed::Speed() {
	defaultSpeed = 0;
	difference = DEFAULT_SPEED;
	newDefaultSpeed = DEFAULT_SPEED;
	maxChangeTime = 1000;
	left = defaultSpeed;
	right = defaultSpeed;
	time = 0;
	changeTime = 0;
}

int Speed::getLeft() {
	return left;
}

int Speed::getRight() {
	return right;
}

void Speed::setDefaultSpeed(int newDefaultSpeed, unsigned long maxChangeTime) {
	this->newDefaultSpeed = newDefaultSpeed;
	this->maxChangeTime = maxChangeTime;
	difference = newDefaultSpeed-defaultSpeed;
}

void Speed::run(Robot *robot, unsigned long delta, bool crossroad) {
	bool *sensors = robot->getDistanceSensors();

	if(defaultSpeed != newDefaultSpeed && changeTime <= maxChangeTime) {
		changeTime += delta;
		if(changeTime >= maxChangeTime) {
			changeTime = maxChangeTime;
		}
		defaultSpeed = (int)(newDefaultSpeed-(1-(changeTime/(float)maxChangeTime))*difference);
	} else {
		difference = 0;
		changeTime = 0;
	}

	unsigned long maxTime = (sensors[1] && crossroad)?STOP_TIME/2:STOP_TIME;
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
	left = (int)(defaultSpeed*factor);
	right = (int)(defaultSpeed*factor);
	if(!crossroad && factor == 0) { // Hupe
		robot->play("C");
	}
}
