#include "module.h"

Module::Module(Robot *robot) {
	this->robot = robot;
}

Robot *Module::getRobot() {
	return robot;
}
