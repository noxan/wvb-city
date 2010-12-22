#ifndef SPEED_H
#define SPEED_H

#include "robot.h"

class Speed {
public:
	static const int DEFAULT_LEFT = 50;
	static const int DEFAULT_RIGHT = 50;

	static const unsigned int STOP_TIME = 500;

	Speed();
	int getLeft();
	int getRight();

	void run(Robot *robot, unsigned long delta, bool crossroad);
private:
	unsigned long time;
	int left;
	int right;
};

#endif
