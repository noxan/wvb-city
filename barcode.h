#define NBIT   6
#define NPERBIT 10
#define QSIZE 70 // >=NBIT * NPERBIT

typedef long unsigned int lint;

#include "queue.h"
#include "robot.h"

#include "module.h"

class Barcode{
	private:
		lint nexttime;
		lint length;
		Queue q;
		int code;
		int cspeed;
		Robot* robot;
	public:
		Barcode(Robot* robot);
		void meassureLength();
		void run(lint time);
		unsigned int getBarcode();
};
