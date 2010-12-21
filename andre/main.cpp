#include "common.h"

int main() {
	unsigned long time = 0; // Zeitpunkt am Anfang eines Schleifendurchlaufs
	unsigned long delta = 0; // Zeit die ein Schleifendurchlauf benötigt
	const unsigned long delay = 10; // Zeit die pro Schleifendurchlauf gewartet wird
	
	OrangutanTime::reset();

	robot.initWait("Ready!");

	while(true) { // Hauptschleife
		time = robot.ms();

		robot.clear();
		robot.update();

		// Linien- und Codemodul"

		//*
		speed.run(&robot, delta);
		line.run(&robot, delta);
		code.run(&robot, delta);

		switch(robot.getStatus()) {
			case Robot::NORMAL:
				robot.print("NORMAL");
				break;
			case Robot::C4:
				robot.print("C4");
				break;
			case Robot::C3SL:
				robot.print("C3SL");
				break;
			case Robot::C3SR:
				robot.print("C3SR");
				break;
			case Robot::C3LR:
				robot.print("C3LR");
				break;
			case Robot::CUNDEF:
				robot.print("CUNDEF");
				break;
		}
//
		//*/
		
		// Testausgabe zum Farbwerte anzeigen

		/*
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
