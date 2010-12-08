#include <pololu/orangutan>
#include <pololu/Pololu3pi/Pololu3pi.h>

#include "robot.h"


Robot::Robot() {
    pol.init(2000); //init
	setDistance(450);
	start("cali");
	lineSensorsCalibrate();
	start("ready");
}

void Robot::update() {
	OrangutanMotors::setSpeeds(speedl, speedr); //update speed
	pol.readLineSensors(sensor_line_raw, IR_EMITTERS_ON); //update line sensors raw
	pol.readLineSensorsCalibrated(sensor_line_calibrate, IR_EMITTERS_ON); //update line sensors calibrate
	for(int i=0;i<5;i++) {
		sensor_line_clean[i] = (sensor_line_calibrate[i] > 250 ? LINE : BACK);;
	}	sensor_distance[0] = OrangutanAnalog::read(6)<getDistance();
	sensor_distance[1] = OrangutanAnalog::read(7)<getDistance();
}

void Robot::start() {
	start("");
}

void Robot::start(const char *str) {
	clear();
	print("Press B!");
	move(0, 1);
	print(str);
	setSpeed(0, 0);
	update();
	waitForPress(BUTTON_B);
}

//speed set
void Robot::setSpeedLeft(int left) {
	if(left>-256 && left<256) {
		this->speedl = left;
	}
}
void Robot::setSpeedRight(int right) {
	if(right>-256 && right<256) {
		this->speedr = right;
	}
}
void Robot::setSpeed(int left, int right) {
	this->setSpeedLeft(left);
	this->setSpeedRight(right);
}

//speed get
int Robot::getSpeedLeft() {
	return speedl;
}
int Robot::getSpeedRight() {
	return speedr;
}
int Robot::getSpeedAverage() {
	return (int)(speedr+speedl)/2;
}

//sensor line
unsigned int *Robot::getLineSensorsRaw() {
	return sensor_line_raw;
}
unsigned int *Robot::getLineSensorsClean() {
	return sensor_line_clean;
}
unsigned int *Robot::getLineSensorsCalibrate() {
	return sensor_line_calibrate;
}

//sensor line cali
void Robot::lineSensorsCalibrate() {
	for(int counter=0;counter<80;counter++) {
        if(counter < 20 || counter >= 60) {
            OrangutanMotors::setSpeeds(30, -30);
        } else {
       	    OrangutanMotors::setSpeeds(-30, 30);
		}
		pol.calibrateLineSensors();
        OrangutanTime::delayMilliseconds(20);
    }
	OrangutanMotors::setSpeeds(0, 0);
}

//sensor distance
void Robot::setDistance(unsigned int dist) {
	this->dist = dist;
}
unsigned int Robot::getDistance() {
	return dist;
}
bool *Robot::getDistanceSensors() {
	return sensor_distance;
}

//lcd
void Robot::print(long value) {
	OrangutanLCD::print(value);
}
void Robot::print(const char *str) {
	OrangutanLCD::print(str);
}
void Robot::clear() {
	OrangutanLCD::clear();
}
void Robot::move(unsigned char x, unsigned char y) {
	OrangutanLCD::gotoXY(x, y);
}

//time
void Robot::delay(unsigned int milliseconds) {
	OrangutanTime::delayMilliseconds(milliseconds);
}
unsigned long Robot::ms() {
	return OrangutanTime::ms();
}

//buttons
unsigned char Robot::waitForPress(unsigned char buttons) {
	return OrangutanPushbuttons::waitForPress(buttons);
}
unsigned char Robot::waitForRelease(unsigned char buttons) {
	return OrangutanPushbuttons::waitForRelease(buttons);
}
unsigned char Robot::waitForButton(unsigned char buttons) {
	return OrangutanPushbuttons::waitForButton(buttons);
}
unsigned char Robot::isPressed(unsigned char buttons) {
	return OrangutanPushbuttons::isPressed(buttons);
}
