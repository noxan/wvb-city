#include "speed.h"


SpeedModule::SpeedModule(Robot* robot, LineModule* line) {
	this->robot = robot;
	this->line = line;
	nexttime = 0;
	speed = 0;
	setMaxSpeed(0);
}

void SpeedModule::setMaxSpeed(int maxspeed) {
	this->maxspeed = maxspeed;
}

void SpeedModule::run(unsigned long int time, bool straight, bool right) {
	if(time<nexttime)
		return;
	nexttime = time + 10; //alle 10 ms: reicht aus, um bei 60 zu bremsen.

//eventuell intelligenter machen: progressives Bremsen => Bei kurzer Zeit Geschwindigkeitsanpassung, annsonsten Vollbremsung.
	bool* s = robot->getDistanceSensors();
	if(speed>maxspeed) speed--;
	else speed++;
	//TODO: ist sensor 1 tatsaechlich der nach vorne?
	if(straight && s[1]) speed-=2;
	if(right && s[0]) speed-=2; //Eventuell hier Konstanten einfuegen oder nexttime schneller berechnen.
	if(speed<0) speed = 0;

	line->setSpeed(speed, speed);
}
