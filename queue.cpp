#include "queue.h"

Queue::Queue() {
	for(int i=0; i<QSIZE; i++) q[i]=0;
	cidx = 0;
}

void Queue::enqueue(lint v) {
	q[cidx]=v;
	cidx++;
	cidx%=QSIZE;
}

lint Queue::operator[](int idx) {
	return q[(cidx + idx)%QSIZE];
}
