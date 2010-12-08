#ifndef MODULE_H
#define MODULE_H

#include "robot.h"

class Module {
private:
	Robot *robot;
public:
	Module(Robot *robot);
	
	Robot *getRobot();

	virtual void run(unsigned long time) = 0;
};

#endif
