#include <stdlib.h>
#include "crossroad.h"
#include "common.h"

Crossroad::Crossroad() {
	status = BEFORE;
	counter = 0;
	option = 0;
	time = 0;
	maxOption = 0;
}

void Crossroad::choose(unsigned int type) {
	int r = 0;
	switch(type) {
		case Robot::C4:
			r = rand()%3;
			if(r == 0) {
				setRight();
			} else if(r == 1) {
				setLeft();
			} else {
				setStraight();
			}
			break;
		case Robot::C3SL:
			r = rand()%2;
			if(r == 0) {
				setLeft();
			} else {
				setStraight();
			}
			break;
		case Robot::C3SR:
			r = rand()%2;
			if(r == 0) {
				setRight();
			} else {
				setStraight();
			}
			break;
		case Robot::C3LR:
			r = rand()%2;
			if(r == 0) {
				setRight();
			} else {
				setLeft();
			}
			break;
	}
}

void Crossroad::run(Robot *robot, unsigned long delta) {
//	if((speed.getLeft()+speed.getRight())/2 != 0) {
		unsigned int *sensors = robot->getLineSensorsClean();

		if(counter == maxOption) {
			counter = 0;
			robot->setStatus(Robot::NORMAL);
			return;
		}

		if(status == BEFORE) {
			if(!(sensors[0] & Robot::LINE || sensors[4] & Robot::LINE)) {
				time = 0;
				if(sensors[2] & Robot::LINE) {
					robot->setSpeed(speed.getLeft(), speed.getRight());	
				} else if(sensors[1] & Robot::LINE) {
					robot->setSpeed((int)(speed.getLeft()*0.83), (int)(speed.getRight()*1.16));
				} else if(sensors[3] & Robot::LINE) {
					robot->setSpeed((int)(speed.getLeft()*1.16), (int)(speed.getRight()*0.83));
				}
			} else {
				time += delta;
				if(time > 30) {
					time = 0;
					status = CROSS1;
				}
			}
		} else if(status == CROSS1) {
			if(sensors[0] & Robot::LINE || sensors[4] & Robot::LINE) {
				time = 0;
				if(sensors[2] & Robot::LINE) {
					robot->setSpeed(speed.getLeft(), speed.getRight());	
				} else if(sensors[1] & Robot::LINE) {
					robot->setSpeed((int)(speed.getLeft()*0.83), (int)(speed.getRight()*1.16));
				} else if(sensors[3] & Robot::LINE) {
					robot->setSpeed((int)(speed.getLeft()*1.16), (int)(speed.getRight()*0.83));
				}
			} else {
				time += delta;
				if(time > 30) {
					time = 0;
					status = CROSS2;
				}
			}
		} else if(status == CROSS2) {
			if(options[counter] == STRAIGHT) {
				counter++;
				status = BEFORE;
			} else if(options[counter] == LEFT) {
				robot->setSpeed(speed.getLeft()/-2, speed.getRight());
				if(sensors[2] & Robot::BACK) {
					time += delta;
					if(time > 30) {
						time = 0;
						status = CROSS3;
					}
				} else {
					time = 0;
				}
			} else {
				robot->setSpeed(speed.getLeft(), speed.getRight()/-2);
				if(sensors[2] & Robot::BACK) {
					time += delta;
					if(time > 30) {
						time = 0;
						status = CROSS3;
					}
				} else {
					time = 0;
				}
			}
		} else if(status == CROSS3) {
			if(options[counter] == LEFT) {
				robot->setSpeed(speed.getLeft()/-2, speed.getRight());
				if(sensors[2] & Robot::LINE) {
					time += delta;
					if(time > 30) {
						time = 0;
						status = BEFORE;
						counter++;
					}
				} else {
					time = 0;
				}
			} else {
				robot->setSpeed(speed.getLeft(), speed.getRight()/-2);
				if(sensors[2] & Robot::LINE) {
					time += delta;
					if(time > 30) {
						time = 0;
						status = BEFORE;
						counter++;
					}
				} else {
					time = 0;
				}
			}
		}

		if(option == LEFT) {
			robot->print("LE");
		} else if(option == RIGHT) {
			robot->print("RI");
		} else if(option == STRAIGHT) {
			robot->print("ST");
		}
//	}
}

void Crossroad::runUndef(Robot *robot, unsigned long delta) { // TODO
	unsigned int *sensors = robot->getLineSensorsClean();

	counter += delta;

	if(sensors[2] & Robot::LINE) {
		robot->setSpeed(speed.getLeft(), speed.getRight());	
	} else if(sensors[1] & Robot::LINE) {
		robot->setSpeed((int)(speed.getLeft()*0.5), (int)(speed.getRight()*1.7));
	} else if(sensors[3] & Robot::LINE) {
		robot->setSpeed((int)(speed.getLeft()*1.7), (int)(speed.getRight()*0.5));
	} else {
		robot->setSpeed(speed.getLeft(), -speed.getRight());
	}

	robot->print("UD");

	if(counter > 3000) {
		counter = 0;
		robot->setStatus(Robot::NORMAL);
	}
}

void Crossroad::setRight() {
	option = RIGHT;
	options[0] = RIGHT;
	maxOption = 1;
}

void Crossroad::setLeft() {
	option = LEFT;
	options[0] = STRAIGHT;
	options[1] = LEFT;
	options[2] = STRAIGHT;
	maxOption = 3;
}

void Crossroad::setStraight() {
	option = STRAIGHT;
	options[0] = STRAIGHT;
	options[1] = STRAIGHT;
	maxOption = 2;
}


