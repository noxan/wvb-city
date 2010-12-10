#ifndef ROBOT_H
#define ROBOT_H

#include <pololu/orangutan>
#include <pololu/Pololu3pi/Pololu3pi.h>

#define CALI_CODE 100
#define CALI_LINE 350

class Robot {
public:
	static const unsigned int BACK = 1;
	static const unsigned int CODE = 2;
	static const unsigned int LINE = 4;

	static const unsigned int STRAIGHT = 0;
	static const unsigned int RIGHT = 1;
	static const unsigned int LEFT = 3;
    //constructor
	Robot();
    //update
	void update();
	//speed set
	void setSpeedLeft(int left);
	void setSpeedRight(int right);
	void setSpeed(int left, int right);
	//speed get
	int getSpeedLeft();
	int getSpeedRight();
	int getSpeedAverage();
    //sensor dist
    bool *getDistanceSensors();
	//sensor line
	unsigned int *getLineSensorsRaw();
	unsigned int *getLineSensorsClean();
	unsigned int *getLineSensorsCalibrate();

	unsigned int getDirection();
	// sensor line cali
	void lineSensorsCalibrate();
	//lcd
	void print(long value);
	void print(const char *str);
	void print(const char *line1, const char *line2);
	void clear();
	void move(unsigned char x, unsigned char y);
	//time
	void delay(unsigned int milliseconds);
	unsigned long ms();
	//buttons
	unsigned char waitForPress(unsigned char buttons);
	unsigned char waitForRelease(unsigned char buttons);
	unsigned char waitForButton(unsigned char buttons);
	unsigned char isPressed(unsigned char buttons);
	//buzzer
	void play(const char *sequence);
	//
	void initWait();
	void initWait(const char *line1);
	void initWait(const char *line1, const char *line2, unsigned char buttons);
private:
    Pololu3pi pol;
	int speedl; //speed left
    int speedr; //speed right
	bool sensor_distance[2]; //sensor distance
	unsigned int sensor_line_raw[5]; //sensor line raw
	unsigned int sensor_line_clean[5]; //sensor line clean
	unsigned int sensor_line_calibrate[5]; //sensor line calibrate
    //distance (internal)
	unsigned int getDistance();
	void setDistance(unsigned int dist);
	unsigned int dist;
};

#endif
