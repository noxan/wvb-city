#define QSIZE 70 // >=NBIT * NPERBIT
typedef long unsigned int lint;

class Queue{
	private:
		lint q[QSIZE];
		int cidx;
	public:
		Queue();
		void enqueue(lint v);
		lint operator[](int idx);
};
