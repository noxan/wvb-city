#include "robot.h"
#include <stdlib.h>

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
	unsigned long WAIT_TIME = 500; //Zeit die nach einem Rechts-vor-Links-Erkenntnisende gewartet wird
	unsigned long time = 0;
	unsigned long remainingtime = 0;

	void runSpeed(Robot *robot, unsigned long delta, bool lookright);
}

namespace distance {
	unsigned long distance = 0;
	
	void runDistance(unsigned long delta);
}

namespace code { // Codemodul
	// Konstanten für die Position des Codemoduls
	static const unsigned int NOCODE = 1; // Kein Code
	static const unsigned int BEGIN = 2; // Codeanfangsstrich
	static const unsigned int CODE = 3; // Code
	static const unsigned int END = 4; // Codeendstrich
	static const unsigned int steplength = 50*10; //10 ms mit Geschwindigkeit 50
	static const unsigned int minSteps = 8; //#der grauen Schrittlaengen, die uberschritten werden muss, es als Markierung zu sehen
	unsigned long steps = 0; // Anzahl der Schritte seit der nur noch Codefarbe erkannt wurde
	unsigned long lastdistance = 0; // Zu welcher Zeit wurde das letzte mal Code gelesen?

	unsigned int status = NOCODE; // Status des Codemoduls

	int code = -1; // der eingelesene Code, -1 heißt dass kein Code gelesen wurde
	
	unsigned int values[200]; // Alle eingelesenen Farbewerte
	unsigned int index = 0; // Index des nächste Farbwertes
	unsigned int average[4];

	void runCode(Robot *robot, unsigned long curdistance); // Zum Ausführen des Codemodul
}

namespace line { // Linienmodul
	// Konstanten für den Status des Linienmoduls
	static const unsigned int FORWARD = 0; // Vorwärts
	static const unsigned int LEFT = 1; // Links Teil 1
	static const unsigned int LEFT2 = 2; // Links Teil 2
	static const unsigned int RIGHT = 3; // Rechts Teil 1
	static const unsigned int RIGHT2 = 4; // Rechts Teil 2
	static const unsigned int MAXTIME = 50; //Wie viele ms duerfen max. nach dem Erkennen, dass RIGHT oder LEFT fehlerhaft sein koennte, vergehen
	unsigned int status = FORWARD; // Aktueller Status des Linienmoduls
	unsigned int time = 0; //Wie lange sind wir schon in einem potentieller-Fehler-Zustand?

	void runLine(Robot *robot, unsigned long delta); // Zum Ausführen des Linienmoduls
}


namespace crossroads { // Linienmodul
	// Konstanten für den Status des Linienmoduls
	static const unsigned int FORWARD = 0; // Vorwärts
	static const unsigned int LEFT = 1; // Links Teil 1
	static const unsigned int LEFT2 = 2; // Links Teil 2
	static const unsigned int RIGHT = 3; // Rechts Teil 1
	static const unsigned int RIGHT2 = 4; // Rechts Teil 2
	static const unsigned int STRAIGHT = 5; // Ueberqueren
	int cnt = 0; int decision = 0;
	unsigned int status = FORWARD; // Aktueller Status des Linienmoduls
	bool active = false;

	void runLine(Robot *robot, unsigned long delta); // Zum Ausführen des Linienmoduls
}


int main() {  
	unsigned long time = 0; // Zeitpunkt am Anfang eines Schleifendurchlaufs
	unsigned long delta = 1; // Zeit die ein Schleifendurchlauf benötigt
	const unsigned long delay = 1; // Zeit die pro Schleifendurchlauf gewartet wird

	Robot robot;

	robot.initWait("Ready!");


	crossroads::decision=5;
	crossroads::active=false;
	while(true) { // Hauptschleife
		time = robot.ms();

		robot.clear();
		robot.update();

		// Linien- und Codemodul"

		//*
		
		if(!crossroads::active) {
			distance::runDistance(delta);
			code::runCode(&robot, distance::distance);
			line::runLine(&robot, delta);
			speed::runSpeed(&robot, delta, false);
			const int c = code::code;
			if(false) if(c==0b00 or c==0b01 or c==0b10 or c==0b11) { //TODO: Bitmasken weitergeben fuer moegliche Richtungen? //TODO: statische Ausdruecke
				robot.play("A");
				int ndecision = -1;
				if(c==0b01)
					ndecision = 2; //nicht nach rechts
				else if(c==0b10)
					ndecision = 0; //nicht geradeaus
				else if(c==0b11)
					ndecision = 1; //nicht nach links
				int decision = 0;
				//do
				//	decision = rand()%3;
				//while(decision == ndecision);

				//worstcasesicherheit = Keine Randomisierung ;)
				if(ndecision==-1)
					decision = rand()%3;
				else {
					decision = rand()%2;
					if(decision>=ndecision) decision++;
				}
				
				crossroads::cnt = 0;
				crossroads::decision = decision;
				crossroads::active = true;
			}
		} else if (crossroads::active) {
			speed::runSpeed(&robot, delta, true);
			crossroads::runLine(&robot, delta);
		}
		
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
	void runSpeed(Robot *robot, unsigned long delta, bool lookright) {
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
		if(lookright) {
			if(sensors[1])
				remainingtime = WAIT_TIME;
			else {
				if(remainingtime>=delta)
					remainingtime -= delta;
				else
					remainingtime = 0;
			}
			if(remainingtime>0)
				time = maxTime;
		}
		float factor = 1-(time/(float)maxTime);
		speed::left = (int)(speed::DEFAULT_LEFT*factor);
		speed::right = (int)(speed::DEFAULT_RIGHT*factor);
	}
}

namespace code { // Codemodul
	void runCode(Robot *robot, unsigned long curdistance) {
		if(curdistance < lastdistance + steplength) //wir sind zufrueh
			return;
		lastdistance += steplength;
		
		unsigned int *sensors = robot->getLineSensorsClean();
		
		//unsigned long minSteps = (unsigned long)(((float)common::MAX_TIME/robot->getSpeedAverage())*common::MAX_TIME);
		// Was wenn wir gerade stehen? Division durch 0

		

		if(line::status == line::FORWARD) {
			if(status == NOCODE) {
				robot->print("N");
				if((sensors[4] & Robot::CODE)) {
					if(++steps >= minSteps) {
						status = BEGIN;
						steps = 0;
					}
				} else {
					steps = 0;
				}
			} else if(status == BEGIN) {
				robot->print("B");
				if(!(sensors[4] & Robot::CODE)) {
					index = 0;
					status = CODE;
				}
			} else if(status == CODE) {
				robot->print("C");
				unsigned int *raw = robot->getLineSensorsCalibrate();

				while(lastdistance<=curdistance) { //solange wir zeitlich im Rueckstand sind
					// code lesen
					if(index < 200) { // falls noch speicher da ist
						values[index++] = raw[4];
					} else { //200 Werte sind zu Lang fuer einen Code
						status = NOCODE;
					}
					lastdistance += steplength;
				}

				// code ende?
				if((sensors[4] & Robot::CODE)) {
					steps++;
					if(steps >= minSteps) {
						status = END;
						steps = 0;
						// code berechnen
						code = 0;
						
						for(unsigned int i = 0; i < 4; i++) {
							unsigned long sum = 0;
							for(unsigned int j = 0; j < (int)(index/4.0); j++) {
								sum += values[(int)(i*(index/4.0)+j)];
							}
							average[i] = (unsigned int)(sum/(index/4.0));
							if(average[i] > 400) {
								code |= (1<<i);
							}
						}
					}
				} else {
					steps = 0;
				}	
			} else if(status == END) {
				robot->print("E");
				if(!(sensors[4] & Robot::CODE)) {
					status = NOCODE;
					robot->play("H");
					// Ergebnisausgabe des Codes
					
					robot->setSpeed(0,0);
					robot->update();
					robot->clear();
					robot->print(code);
					
					robot->delay(1000);
					robot->clear();
					robot->print("i=");
					robot->print(index);
					robot->delay(1000);
					for(int i = 0; i < 4; i++) {
						robot->clear();
						robot->print("avg=");
						robot->print(i);
						robot->move(0,1);
						robot->print(average[i]);
						robot->delay(500);
					}
					
				}
			}
		} else {
			robot->print("N");
		}
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
			if((sensors[0] & Robot::BACK) and (sensors[2] & Robot::LINE)) { //Vllt war es eine Kreuzung mit ueberlesenen Barcode? => Geradeaus, wenn moeglich
				time += delta;
				if(time>=MAXTIME) {
					time = 0;
					status = FORWARD;
				}
			} else {
				time = 0;
			}
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
			if((sensors[4] & Robot::BACK) and (sensors[2] & Robot::LINE)) { //Vllt war es eine Kreuzung mit ueberlesenen Barcode? => Geradeaus, wenn moeglich
				time += delta;
				if(time>=MAXTIME) {
					time = 0;
					status = FORWARD;
				}
			} else {
				time = 0;
			}
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



namespace crossroads {
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
					if(cnt==0) { //erstes Eck
						if(decision == cnt)
							status = RIGHT; //TODO: Fehlererkennung: was, wenn rechts nicht moeglich ist?
						else
							status = STRAIGHT; //TODO: BarcodeFehlererkennung: was, wenn geradeaus nicht moeglich ist?
					} else { //andere Ecken(geradeaus oder links)
						if(decision == cnt)
							status = STRAIGHT; //TODO: BarcodeFehlererkennung: was, wenn geradeaus nicht moeglich ist?
						else
							status = LEFT;
					}
				}
			}
		} else if(status == LEFT) {
			robot->setSpeed(0, speed::right);
			if(sensors[2] & Robot::BACK) {
				status = LEFT2;
			}		
		} else if(status == LEFT2) {
			robot->setSpeed(speed::left/-2, speed::right);
			if(sensors[2] & Robot::LINE) {
				status = FORWARD;
				cnt++;
			}
		} else if(status == RIGHT) {
			robot->setSpeed(speed::left, 0);
			if(sensors[2] & Robot::BACK) {
				status = RIGHT2;
			}	
		} else if(status == RIGHT2) {	
			robot->setSpeed(speed::left, speed::right/-2);
			if(sensors[2] & Robot::LINE) {
				status = FORWARD;
				cnt++;
			}
		} else if(status == STRAIGHT) {	
			robot->setSpeed(speed::left, speed::right);
			if(sensors[0] & sensors[4] & Robot::BACK) {
				status = FORWARD;
				cnt++;
			}
		}
		if(cnt==decision) {
			active = false;
			robot->play("C");
		}
	}
}


namespace distance{
	void runDistance (unsigned long delta) {
		distance += delta*(speed::left + speed::right)/2;
	}
}
