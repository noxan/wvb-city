#include "robot.h"
#include "line.h"

class SpeedModule{
	private:
		int maxspeed;
		unsigned long int nexttime;
		int speed;
		Robot* robot;
		LineModule* line;
	public:
		SpeedModule(Robot* robot, LineModule* line);
		void run(unsigned long int time, bool straight, bool right);
		void setMaxSpeed(int speed);
};
