#ifndef CODE_H
#define CODE_H

#include "robot.h"

class Code {
public:
	static const int CODE_EMPTY = -1;
	static const int CODE_ERROR = -2;

	static const unsigned long MAX_TIME = 40;

	static const unsigned int VALUES = 80;

	static const unsigned int NOCODE = 0;
	static const unsigned int BEGIN = 1;
	static const unsigned int CODE = 2;

	static const int CMASK = 0x3;
	static const int C4 = 0x0;
	static const int C3SR = 0x1;
	static const int C3SL = 0x2;
	static const int C3LR = 0x3;

	Code();

	int getCode();
	unsigned int getStatus();
	void run(Robot *robot, unsigned long delta);
private:
	unsigned int dif; // code anfang und ende, abweichungen von codefarbe
	unsigned long time; // zeitzähler allgemein
	unsigned long leftcnt; // während code auf linker seite schwarz zählen
	unsigned int status;
	int code;
	unsigned int nextTime;
	unsigned int values[VALUES];
	unsigned int index;
	unsigned int average[4];
};

#endif
