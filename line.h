#ifndef LINE_H
#define LINE_H

#include "module.h"

class LineModule : public Module {
private:
	int speedl;
	int speedr;
	int status;

	//Kreuzungsspam
	int decision;
	int cnt;
	unsigned long nexttime;
public:
	static const int FORWARD = 0;
	static const int LEFT = 1;
	static const int LEFT2 = 2;
	static const int RIGHT = 3;
	static const int RIGHT2 = 4;
	static const int STRAIGHT = 5;

	LineModule(Robot *robot);

	void setSpeed(int left, int right);
	void run(unsigned long time); //time statt delta

	//am Besten folgende Spamfunktionen in zweite Klasse auslagern oder mit den anderen nicht-redundant integrieren
	void run2(unsigned long time);
	void setTarget(int decision);
	bool hasFinished();

};

#endif
