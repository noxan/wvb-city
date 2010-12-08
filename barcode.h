#define NBIT   6
#define NPERBIT 10
#define QSIZE 70 // >=NBIT * NPERBIT

typedef long unsigned int lint;

#include "queue.h"
#include "robot.h"

#include "module.h"

class Barcode : public Module {
	private:
		lint nexttime;
		lint length;
		Queue q;
		int code;
		int cspeed;
		Robot* r;
	public:
		Barcode(Robot* robot);
		void meassureLength();
		void run(lint time);
		unsigned int getBarcode();
};
