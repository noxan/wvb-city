#include "robot.h"

namespace common { // Allgemeines
	// Zeit die auf der Codefarbe verbracht werden muss, damit Code erkannt wird
	// Wird automatisch an die Geschwindigkeit angepasst
	static const unsigned long MAX_TIME = 50; // 50 ms 50 speed --> 25 ms 100 speed?
}

namespace speed { // Geschwindigkeitsmodul --> TODO
	static const unsigned int DEFAULT_LEFT = 50;
	static const unsigned int DEFAULT_RIGHT = 50;
	int left = DEFAULT_LEFT; // Geschwindigkeit des linken Rades
	int right = DEFAULT_RIGHT; // Geschwindigkeit des rechten Rades

	unsigned long MAX_TIME = 500;
	unsigned long time = 0;

	void runSpeed(Robot *robot, unsigned long delay);
}

namespace code { // Codemodul
	// Konstanten für die Position des Codemoduls
	static const unsigned int NOCODE = 1; // Kein Code
	static const unsigned int BEGIN = 2; // Codeanfangsstrich
	static const unsigned int CODE = 3; // Code
	static const unsigned int END = 4; // Codeendstrich

	unsigned long time = 0; // Zeit seit nur noch Codefarbe erkannt wurde

	unsigned int status = NOCODE; // Status des Codemoduls

	int code = -1; // der eingelesene Code, -1 heißt dass kein Code gelesen wurde
	unsigned int nextTime = 0; // Zeit bis zum nächsten mal Farbwert für Code einlesen
	unsigned int bitTime = 0; // Zeit die das erste Bit des Codes in etwa zum Überfahren benötigt
	unsigned int values[40]; // Alle eingelesenen Farbewerte
	unsigned int index = 0; // Index des nächste Farbwertes
	unsigned int average[4];

	void runCode(Robot *robot, unsigned long delta); // Zum Ausführen des Codemodul
}

namespace line { // Linienmodul
	// Konstanten für den Status des Linienmoduls
	static const unsigned int FORWARD = 0; // Vorwärts
	static const unsigned int LEFT = 1; // Links Teil 1
	static const unsigned int LEFT2 = 2; // Links Teil 2
	static const unsigned int RIGHT = 3; // Rechts Teil 1
	static const unsigned int RIGHT2 = 4; // Rechts Teil 2

	unsigned int status = FORWARD; // Aktueller Status des Linienmoduls

	void runLine(Robot *robot, unsigned long delta); // Zum Ausführen des Linienmoduls
}

int main() {  
	unsigned long time = 0; // Zeitpunkt am Anfang eines Schleifendurchlaufs
	unsigned long delta = 0; // Zeit die ein Schleifendurchlauf benötigt
	const unsigned long delay = 10; // Zeit die pro Schleifendurchlauf gewartet wird
	
	OrangutanTime::reset();

	Robot robot;

	robot.initWait("Ready!");

	while(true) { // Hauptschleife
		time = robot.ms();

		robot.clear();
		robot.update();

		// Linien- und Codemodul"

		//*
		speed::runSpeed(&robot, delay);
		line::runLine(&robot, delay);
		code::runCode(&robot, delay);
//
		//*/
		
		// Testausgabe zum Farbwerte anzeigen

		/*
		unsigned int *sensors = robot.getLineSensorsClean();
		robot.print(sensors[0]);
		robot.print(" ");
		robot.print(sensors[1]);
		robot.print(" ");
		robot.print(sensors[2]);
		robot.move(0,1);

		unsigned int *sensors2 = robot.getLineSensorsCalibrate();
		robot.print(sensors2[0]);
		robot.print(" ");
		robot.print(sensors2[1]);
		//*/

		robot.delay(delay);

		delta = robot.ms() - time; // Berechnet die Zeit die ein Schleifendurchlauf benötigt hat
	}
	return 0;
}



namespace speed {
	void runSpeed(Robot *robot, unsigned long delta) {
		bool *sensors = robot->getDistanceSensors();
		unsigned long maxTime = MAX_TIME*2*50/(speed::DEFAULT_LEFT+speed::DEFAULT_RIGHT);
		if(sensors[0]) {
			if(time+delta <= maxTime) {
				time += delta;
			} else {
				time = maxTime;
			}
		} else {
			if(time>=delta) {
				time -= delta;
			} else {
				time = 0;
			}
		}
		float factor = 1-(time/(float)maxTime);
		speed::left = (int)(speed::DEFAULT_LEFT*factor);
		speed::right = (int)(speed::DEFAULT_RIGHT*factor);
	}
}

namespace code { // Codemodul
	void runCode(Robot *robot, unsigned long delta) {
		unsigned int *sensors = robot->getLineSensorsClean();
		
		unsigned long maxTime = (unsigned long)(((float)common::MAX_TIME/robot->getSpeedAverage())*common::MAX_TIME);

		if(line::status == line::FORWARD) {
			if(status == NOCODE) {
				robot->print("N");
				if((sensors[0] & Robot::CODE)) {
					time += delta;
					bitTime += delta;
					if(time >= maxTime) {
						status = BEGIN;
						time = 0;
					}
				} else {
					bitTime = 0;
					time = 0;
				}
			} else if(status == BEGIN) {
				robot->print("B");
				bitTime += delta;
				if(!(sensors[0] & Robot::CODE)) {
					index = 0;
					nextTime = 0;
					status = CODE;
				}
			} else if(status == CODE) {
				robot->print("C");
				nextTime += delta;
				unsigned int *raw = robot->getLineSensorsCalibrate();
				
				// code lesen
				while(nextTime >= bitTime/10 && index < 40) { // falls wieder Zeit ist
					nextTime -= (bitTime/10); // delta
					values[index++] = raw[0];
				}

				// code ende?
				if((sensors[0] & Robot::CODE)) {
					time += delta;
					if(time >= maxTime) {
						status = END;
						time = 0;
						// code berechnen
						code = 0;
						for(unsigned int i = 0; i < 4; i++) {
							unsigned long sum = 0;
							for(unsigned int j = 0; j < (int)(index/4.0); j++) {
								sum += values[i*10+j];
							}
							unsigned int bit = (unsigned int)(sum/(index/4.0));
							average[i] = bit;
							if(bit > 400) {
								code |= (1<<i);
							}
						}
					}
				} else {
					time = 0;
				}	
			} else if(status == END) {
				robot->print("E");
				if(!(sensors[0] & Robot::CODE)) {
					status = NOCODE;
					// Ergebnisausgabe des Codes
					/*
					robot->setSpeed(0,0);
					robot->update();
					robot->delay(1000);
					robot->clear();
					robot->print("i=");
					robot->print(index);
					robot->delay(1000);
					for(int i = 0; i < 4; i++) {
						robot->clear();
						robot->print("avg=");
						robot->print(average[i]);
						robot->delay(1000);
					}
					robot->delay(1000);
					for(unsigned int i = 0; i < index; i++) {
						robot->clear();
						robot->print(values[i]);
						robot->delay(250);
					}//*/
					//
				}
			}
		} else {
			robot->print("N");
		}

		robot->print(" ");
		robot->print(bitTime);
		robot->print(" ");
		robot->print(code);
		robot->move(0,1);
	}
}

namespace line {
	void runLine(Robot *robot, unsigned long delta) {
		unsigned int *sensors = robot->getLineSensorsClean();
		if(status == FORWARD) {
			if(sensors[2] & Robot::LINE) {
				robot->setSpeed(speed::left, speed::right);	
			} else if(sensors[1] & Robot::LINE) {
				robot->setSpeed((int)(speed::left*0.83), (int)(speed::right*1.16));
			} else if(sensors[3] & Robot::LINE) {
				robot->setSpeed((int)(speed::left*1.16), (int)(speed::right*0.83));
			}

			if(code::status == code::NOCODE) {
				if((sensors[0] & Robot::LINE)) {
					status = LEFT;
				} else if((sensors[4] & Robot::LINE)) {
					status = RIGHT;
				}
			}
		} else if(status == LEFT) {
			robot->setSpeed(speed::left, speed::right);
			if(sensors[2] & Robot::BACK) {
				status = LEFT2;
			}		
		} else if(status == LEFT2) {
			robot->setSpeed(speed::left/-2, speed::right);
			if(sensors[2] & Robot::LINE) {
				status = FORWARD;
			}
		} else if(status == RIGHT) {
			robot->setSpeed(speed::left, speed::right);
			if(sensors[2] & Robot::BACK) {
				status = RIGHT2;
			}	
		} else if(status == RIGHT2) {	
			robot->setSpeed(speed::left, speed::right/-2);
			if(sensors[2] & Robot::LINE) {
				status = FORWARD;
			}
		}
	}
}
