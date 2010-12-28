#ifndef ROBOT_H
#define ROBOT_H

#define ROBOT_VERSION 0.1.1

#include <pololu/orangutan>
#include <pololu/Pololu3pi/Pololu3pi.h>

#define CALI_CODE 170
#define CALI_LINE 400

class Robot {
public:
	//status
	static const unsigned int NORMAL = 0; //normal: line + code
	static const unsigned int C4 = 1; //4er kreuzung S|L|R
	static const unsigned int C3SL = 2; //3er kreuzung: S|L
	static const unsigned int C3SR = 3; //3er kreuzung: S|R
	static const unsigned int C3LR = 4; //3er kreuzung: L|R
	static const unsigned int CUNDEF = 5; //undefiniert
	
	static const unsigned int BACK = 1;
	static const unsigned int CODE = 2;
	static const unsigned int LINE = 4;
	
    //constructor
	Robot();
	//random
	void initializeRandom();
    //update
	void update();
	//status
	void setStatus(unsigned int status);
	unsigned int getStatus();
	//speed set
	void setSpeedLeft(int left);
	void setSpeedRight(int right);
	void setSpeed(int left, int right);
	//speed get
	int getSpeedLeft();
	int getSpeedRight();
	int getSpeedAverage();
	int getSpeedAverageAbs();
    //sensor: dist
    bool *getDistanceSensors();
	//sensor: line
	unsigned int *getLineSensorsRaw();
	unsigned int *getLineSensorsClean();
	unsigned int *getLineSensorsCalibrate();
	void lineSensorsCalibrate(); //sensor: line cali
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
	//init user functions
	void initWait();
	void initWait(const char *line1);
	void initWait(const char *line1, const char *line2, unsigned char buttons);
private:
	unsigned int status;
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
