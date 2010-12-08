#include "robot.h"
#include "module.h"

class SpeedModule{
	private:
		int maxspeed;
		Robot* robot;
		LineModule* line;
	public:
		SpeedModule(Robot* robot, LineModule* line);
		void run(unsigned long int time, bool s1, bool s2);
};
