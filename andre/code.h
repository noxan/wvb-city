#ifndef CODE_H
#define CODE_H

#include "robot.h"

class Code {
public:
	static const int CODE_EMPTY = -1;
	static const int CODE_ERROR = -2;

	static const unsigned long MAX_TIME = 50;

	static const unsigned int VALUES = 100;

	static const unsigned int NOCODE = 0;
	static const unsigned int BEGIN = 1;
	static const unsigned int CODE = 2;
	static const unsigned int END = 3;

	static const int CMASK = 0x3;
	static const int C4 = 0x0;
	static const int C3SR = 0x1;
	static const int C3SL = 0x2;
	static const int C3LR = 0x3;
private:
	unsigned long time;
	unsigned int status;
	int code;
	unsigned int nextTime;
	unsigned int values[VALUES];
	unsigned int index;
	unsigned int average[4];
public:
	Code();

	unsigned int getStatus();
	void run(Robot *robot, unsigned long delta);
};

#endif
