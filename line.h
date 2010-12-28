#ifndef LINE_H
#define LINE_H

#include "robot.h"

class Line {
public:
	static const unsigned int FORWARD = 0;
	static const unsigned int LEFT = 1;
	static const unsigned int LEFT2 = 2;
	static const unsigned int RIGHT = 3;
	static const unsigned int RIGHT2 = 4;

	Line();

	unsigned int getStatus();

	void run(Robot* robot, unsigned long delta);
private:
	unsigned int status;
};

#endif
