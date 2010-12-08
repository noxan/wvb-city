#include "line.h"

LineModule::LineModule(Robot *robot) : Module(robot) {
	this->status = FORWARD;
	this->setSpeed(60, 60);
}

void LineModule::setSpeed(int left, int right) {
	this->speedl = left;
	this->speedr = right;
}

void LineModule::run(unsigned long time) {
	//wenn du wirklich einen delay von 10 ms brauchst: dann ist es mE besser, hier etwas per time zu basteln (time<=lasttime+10) statt in der Haupt-while(true) schleife, denn andere Module brauchen eventuell andere delays. Vllt kann man die Module Klasse dementsprechend erweitern
	unsigned int *sensors = getRobot()->getLineSensorsClean();
	getRobot()->clear();
	if(status == FORWARD) {
		getRobot()->print("FORWARD");
		if(sensors[2] > Robot::BACK) {
			getRobot()->setSpeed(speedl, speedr);	
		} else if(sensors[1] > Robot::BACK) {
			getRobot()->setSpeed((int)(speedl*0.83), (int)(speedr*1.16));
		} else if(sensors[3] > Robot::BACK) {
			getRobot()->setSpeed((int)(speedl*1.16), (int)(speedr*0.83));
		}

		if(sensors[0] > Robot::BACK) {
			status = LEFT;
		} else if(sensors[4] > Robot::BACK) {
			status = RIGHT;
		}
	} else if(status == LEFT) {
		getRobot()->print("LEFT");
		getRobot()->setSpeed(speedl, speedr);
		if(sensors[2] == Robot::BACK) {
			status = LEFT2;
		}		
	} else if(status == LEFT2) {
		getRobot()->print("LEFT2");
		getRobot()->setSpeed(-speedl/2, speedr);
		if(sensors[2] > Robot::BACK) {
			status = FORWARD;
		}
	} else if(status == RIGHT) {
		getRobot()->print("RIGHT");
		getRobot()->setSpeed(speedl, speedr);
		if(sensors[2] == Robot::BACK) {
			status = RIGHT2;
		}	
	} else if(status == RIGHT2) {
		getRobot()->print("RIGHT2");		
		getRobot()->setSpeed(speedl, -speedr/2);
		if(sensors[2] > Robot::BACK) {
			status = FORWARD;
		}
	}
}
//-----
/*
decision:
   1
   |
 2- -0
   |
   ^
*/
void LineModule::setTarget(int decision) {
	this->cnt = 0; //Anzahl der Ueberfahrenen Ecken
	this->decision = decision;
}

bool LineModule::hasFinished() {
	return cnt>decision; //Genug Kurven gedreht.
}

void LineModule::run2(unsigned long time) {
	unsigned int *sensors = getRobot()->getLineSensorsClean();
	
	if(status == FORWARD) {
		getRobot()->print("FORWARD");
		if(sensors[2] > Robot::BACK) {
			getRobot()->setSpeed(speedl, speedr);	
		} else if(sensors[1] > Robot::BACK) {
			getRobot()->setSpeed((int)(speedl*0.83), (int)(speedr*1.16));
		} else if(sensors[3] > Robot::BACK) {
			getRobot()->setSpeed((int)(speedl*1.16), (int)(speedr*0.83));
		}

		if(sensors[0] > Robot::BACK) {
			if(cnt==0) { //erstes Eck
				if(decision == cnt)
					status = RIGHT;
				else
					status = STRAIGHT;
			} else { //andere Ecken(geradeaus oder links)
				if(decision == cnt)
					status = STRAIGHT;
				else
					status = LEFT;
			}
		}
	} else if(status == LEFT) {
		getRobot()->print("LEFT");
		getRobot()->setSpeed(speedl, speedr);
		if(sensors[2] == Robot::BACK) {
			status = LEFT2;
		}		
	} else if(status == LEFT2) {
		getRobot()->print("LEFT2");
		getRobot()->setSpeed(-speedl/2, speedr);
		if(sensors[2] > Robot::BACK) {
			status = FORWARD;
			cnt++;
		}
	} else if(status == RIGHT) {
		getRobot()->print("RIGHT");
		getRobot()->setSpeed(speedl, speedr);
		if(sensors[2] == Robot::BACK) {
			status = RIGHT2;
		}	
	} else if(status == RIGHT2) {
		getRobot()->print("RIGHT2");		
		getRobot()->setSpeed(speedl, -speedr/2);
		if(sensors[2] > Robot::BACK) {
			status = FORWARD;
			cnt++;
		}
	} else if(status == STRAIGHT) {
		getRobot()->print("STRAIGHT");		
		getRobot()->setSpeed(speedl, speedr);
		if(sensors[0] == Robot::BACK && sensors[4] == Robot::BACK) {
			status = FORWARD;
			cnt++;
		}
	}
}


