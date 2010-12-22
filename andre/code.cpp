#include "code.h"
#include "common.h"

Code::Code() {
	time = 0;
	status = NOCODE;
	code = CODE_EMPTY;
	nextTime = 0;
	dif = 0;
	index = 0;
}

int Code::getCode() {
	int c = code;
	code = CODE_EMPTY;
	return c;
}

unsigned int Code::getStatus() {
	return status;
}

void Code::run(Robot *robot, unsigned long delta) {
	unsigned int *sensors = robot->getLineSensorsClean();
	unsigned long maxTime = MAX_TIME; //(unsigned long)(((float)MAX_TIME/robot->getSpeedAverage())*MAX_TIME);

	if(line.getStatus() == Line::FORWARD) {
		if(status == NOCODE) {
			robot->print("N");
			if((sensors[4] & Robot::CODE)) {
				time += delta;
				if(time >= maxTime) {
					status = BEGIN;
					dif = 0;
					time = 0;
				}
			} else {
				if(++dif >= 4) {
					time = 0;
					dif = 0;
				}
			}
		} else if(status == BEGIN) {
			robot->print("B");
			if(!(sensors[4] & Robot::CODE)) {
				index = 0;
				time = 0;
				nextTime = 0;
				status = CODE;
			}
		} else if(status == CODE) {
			robot->print("C");
			nextTime += delta;
			unsigned int *raw = robot->getLineSensorsCalibrate();
				// code lesen
			while(nextTime >= 10 && index < VALUES) { // falls wieder Zeit ist
				nextTime -= 10;
				values[index++] = raw[4];
			}
			if(index == VALUES) {
				status = NOCODE;
				code = CODE_ERROR;
				time = 0;
				return;
			}
				// code ende?
			if((sensors[4] & Robot::CODE)) {
				time += delta;
				if(time >= maxTime) {
					status = NOCODE;
					time = 0;
					dif = 0;
					// code berechnen
					code = 0;
					for(unsigned int i = 0; i < 4; i++) {
						unsigned long sum = 0;
						for(unsigned int j = 0; j < (int)(index/4.0); j++) {
							sum += values[(int)(i*(index/4.0)+j)];
						}
						unsigned int bit = (unsigned int)(sum/(index/4.0));
						average[i] = bit;
						if(bit > 300) {
							code |= (1<<i);
						}
					}
					/*
					robot->setSpeed(0,0);
					robot->update();
					for(int i = 0; i < 4; i++) {
						robot->clear();
						robot->print(average[i]);
						robot->delay(1000);
					}
					*/
				}
			} else {
				if(++dif >= 4) {
					time = 0;
					dif = 0;
				}
			}	
		}
	} else {
		robot->print("N");
	}

	robot->print(" ");
	robot->print(index);
	robot->print(" ");
	robot->print(code);
	robot->move(0,1);
}
