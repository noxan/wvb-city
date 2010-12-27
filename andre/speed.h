#ifndef SPEED_H
#define SPEED_H

#include "robot.h"

class Speed {
public:
	static const int DEFAULT_SPEED = 80;

	static const unsigned long STOP_TIME = 500;

	Speed();
	int getLeft();
	int getRight();

	void setDefaultSpeed(int newDefaultSpeed, unsigned long maxChangeTime);

	void run(Robot *robot, unsigned long delta, bool crossroad);
private:
	unsigned long time;
	unsigned long changeTime;
	unsigned long maxChangeTime;
	int left;
	int right;
	int difference;
	int defaultSpeed;
	int newDefaultSpeed;
};

#endif
