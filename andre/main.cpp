#include <stdlib.h>
#include "common.h"

int main() {
	unsigned long time = 0; // Zeitpunkt am Anfang eines Schleifendurchlaufs
	unsigned long delta = 0; // Zeit die ein Schleifendurchlauf benötigt
	const unsigned long delay = 10; // Zeit die pro Schleifendurchlauf gewartet wird
	
	OrangutanTime::reset();

	srand(robot.ms());

	robot.initWait("Ready!");

	while(true) { // Hauptschleife
		time = robot.ms();

		robot.clear();
		robot.update();

		// Linien- und Codemodul"

		//*

		if(robot.getStatus() == Robot::NORMAL) {
			line.run(&robot, delta);
			code.run(&robot, delta);
			speed.run(&robot, delta, false);
			
			int c = code.getCode();
			if(c != -1) {
				robot.setStatus(Robot::CUNDEF);
				if(c == -2) {
					 robot.setStatus(Robot::CUNDEF);
				} else if((c & Code::CMASK) == Code::C4) {
					robot.setStatus(Robot::C4);
				} else if((c & Code::CMASK) == Code::C3SL) {
					robot.setStatus(Robot::C3SL);
				} else if((c & Code::CMASK) == Code::C3SR) {
					robot.setStatus(Robot::C3SR);
				} else if((c & Code::CMASK) == Code::C3LR) {
					robot.setStatus(Robot::C3LR);
				}
				crossroad.choose(robot.getStatus());
			}
		} else {
			crossroad.run(&robot, delta);
			speed.run(&robot, delta, true);	
		}

		robot.print(robot.getStatus());
//
		/*/
		
		// Testausgabe zum Farbwerte anzeigen

		//*
		unsigned int *sensors = robot.getLineSensorsClean();
		robot.print(sensors[2]);
		robot.print(" ");
		robot.print(sensors[3]);
		robot.print(" ");
		robot.print(sensors[4]);
		robot.move(0,1);

		unsigned int *sensors2 = robot.getLineSensorsCalibrate();
		robot.print(sensors2[2]);
		robot.print(" ");
		robot.print(sensors2[4]);
		//*/

		robot.delay(delay);

		delta = robot.ms() - time; // Berechnet die Zeit die ein Schleifendurchlauf benötigt hat
	}
	return 0;
}
