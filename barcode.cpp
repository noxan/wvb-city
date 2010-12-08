#define NBIT   6
#define NPERBIT 10
#define QSIZE 70 // >=NBIT * NPERBIT
#include "barcode.h"
#include "module.h"
//#include "queue.h"


Barcode::Barcode(Robot* robot) : robot(robot){
	nexttime = 0;
	length = 0;
	q = Queue();
	cspeed = 50;
}

unsigned int Barcode::getBarcode() {
	return code;
}

void Barcode::meassureLength() {
	int v = 0;
	lint sTime = 0, eTime = 0;
	int SPEED = 50;
	robot->setSpeed(SPEED, SPEED);
	robot->update();
	bool isOnLine = false;
	unsigned int* sv = robot->getLineSensorsCalibrate();
	for(int i=0; i<3000; i++) {
		v = (v*3+sv[2])/4;
		robot->update();
		if(!isOnLine && v>500) {
			getRobot()->play("D");
			sTime = OrangutanTime::ms();
			isOnLine = true;
		}
		if(isOnLine && v < 500) {
			getRobot()->play("A");
			eTime = OrangutanTime::ms();
			isOnLine = false;
			break;
		}
		getRobot()->delay(1);
	}
	robot->setSpeed(0,0);
	robot->update();
	length = SPEED * (eTime - sTime);
}

void Barcode::run(lint time) {
	if(time>=nexttime) {
		// nexttime = time + std::min(length/((QSIZE-1) * cspeed), 100);
		nexttime = time + (length/((QSIZE-1) * robot->getSpeedAverage()));
		q.enqueue(q[QSIZE-1] + robot->getLineSensorsCalibrate()[4]);
		code = 0;
		for(int b=0; b<NBIT; b++) {
			int s = NPERBIT*b;
			int e = NPERBIT*(b+1)-1;
			int v = (q[e]-q[s])/(NPERBIT-1); //avg in dem Bit
			if(v>700) {//freuen, schwaaarz
				;
			} else if(300<=v && v<=700) { //grau (und hoffentlich nicht wei
				code |= 1<<b;
			} else {
				code = -1;
				return;
			}
		}
	} else
		code = -1;
}
