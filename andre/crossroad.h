#ifndef CROSSROAD_H
#define CROSSROAD_H

#include "robot.h"

class Crossroad {
public:
	static const unsigned int RIGHT = 1;
	static const unsigned int LEFT = 2;
	static const unsigned int STRAIGHT = 4;

	static const unsigned int BEFORE = 0;
	static const unsigned int CROSS1 = 1;
	static const unsigned int CROSS2 = 2;
	static const unsigned int CROSS3 = 3;

	Crossroad();

	void choose(unsigned int type);
	void run(Robot *robot, unsigned long delta);
	void runUndef(Robot *robot, unsigned long delta);
private:
	unsigned int time;
	unsigned int status;
	unsigned int counter;
	unsigned int maxOption;
	unsigned int option;
	unsigned int options[3];

	void setRight();
	void setLeft();
	void setStraight();
};

#endif
