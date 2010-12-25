#include "line.h"
#include "common.h"

Line::Line() {
	status = FORWARD;
}

unsigned int Line::getStatus() {
	return status;
}

void Line::run(Robot *robot, unsigned long delta) {
	unsigned int *sensors = robot->getLineSensorsClean();
	if(status == FORWARD) {
		if(sensors[2] & Robot::LINE) {
			robot->setSpeed(speed.getLeft(), speed.getRight());	
		} else {
			if(sensors[1] & Robot::LINE) {
				robot->setSpeed((int)(speed.getLeft()*0.83), (int)(speed.getRight()*1.16));
			} else if(sensors[3] & Robot::LINE) {
				robot->setSpeed((int)(speed.getLeft()*1.16), (int)(speed.getRight()*0.83));
			}
		}

		if(code.getStatus() == Code::NOCODE) {
			if((sensors[0] & Robot::LINE)) {
				status = LEFT;
			} else if((sensors[4] & Robot::LINE)) {
				status = RIGHT;
			}
		}
	} else if(status == LEFT) {
		robot->setSpeed(speed.getLeft(), speed.getRight());
		if(sensors[2] & Robot::BACK) {
			status = LEFT2;
		}		
	} else if(status == LEFT2) {
		robot->setSpeed(speed.getLeft()/-2, speed.getRight());
		if(sensors[2] & Robot::LINE) {
			status = FORWARD;
		}
	} else if(status == RIGHT) {
		robot->setSpeed(speed.getLeft(), speed.getRight());
		if(sensors[2] & Robot::BACK) {
			status = RIGHT2;
		}	
	} else if(status == RIGHT2) {	
		robot->setSpeed(speed.getLeft(), speed.getRight()/-2);
		if(sensors[2] & Robot::LINE) {
			status = FORWARD;
		}
	}
}
