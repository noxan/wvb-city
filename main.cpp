#include <pololu/OrangutanTime/OrangutanTime.h>
#include <stdlib.h>
#include "module.h"
#include "line.h"
#include "robot.h"
#include "barcode.h"

// handle undefined virtual function
extern "C" void __cxa_pure_virtual() {
	while (1);
}

int main() {
	Robot robot;
	LineModule line(&robot);
	Barcode bar(&robot);
	
	robot.clear();
	robot.print("Press B to start!");
	do {
		bar.meassureLength();
		while(!robot.isPressed(BUTTON_B | BUTTON_C)) {
			robot.clear();
			robot.print("B: retry.", "C: enter.");
			robot.delay(10);
	   }
	} while(robot.isPressed(BUTTON_B));
	
	static const int CROSSROADS = 0;
	static const int NORMAL = 1;
	
	int status = NORMAL;
	int decision = 0;
	//speedModule.setMaxSpeed(60);
	
	srand(robot.getLineSensorsRaw()[0]); //seed
	
	long unsigned int time;
	int code;
	
	while(true) {
		time = robot.ms();
		
		if(status == NORMAL) {
			robot.clear();
			robot.print("NORM");
			
			robot.update();
			
			// handle modules
			//speedModule.run(time, true, false); //passt Geschwindigkeit unter Beruecksichtigung des mittleren Sensors an
			line.run(time);
			bar.run(time);
			
			code = bar.getBarcode();
			
			if(code!=-1 and code&32 and code&1) { //ein paar Prozessortakte sparen ;-)
				if(code==37) robot.play("C");
				//robot.clear();
				//robot.print(code);
				//robot.delay(200);
				//robot.clear();
				//robot.print(code);
				//robot.delay(150);
				
				if(code==43) { //Schnellfahrcode
					//speedModule.setMaxSpeed(100);
				} else if(code == 33 or code==35 or code==37 or code==39 or code==41) { //Kreuzung oder Abzweigung
					int ndecision = -1; //Diese Richtungsentscheidung darf man nicht fahren, da keine strasse da
					if(code==35)
						ndecision = 0; //nicht nach rechts
					else if(code==37)
						ndecision = 1; //nicht geradeaus
					else if(code==33)
						ndecision = 2; //nicht nach links
					do
						decision = rand()%3;
					while(decision == ndecision);
					//status = CROSSROADS;
					//speedModule.setMaxSpeed(30); //->Langsam fahren in Kreuzungen
				}
			}
		} else if(status == CROSSROADS) {
			robot.clear();
			robot.print("CR");
			//speedModule.run(time, true, true); //passt Geschwindigkeit unter Beruecksichtigung beider Sensoren an => rechts vor links
			line.setTarget(decision);
			line.run2(time);
			if(line.hasFinished()) {
				status = NORMAL;
				//speedModule.setMaxSpeed(60); // auf Strassen schneller fahren
			}
		}
	}
	return 0;
}
