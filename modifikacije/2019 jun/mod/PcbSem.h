
#ifndef HFILES_PCBSEM_H_
#define HFILES_PCBSEM_H_
#include "queue.h"
#include "timeQue.h"
#include "Semaphor.h"
class PcbSem{
public:
	PcbSem(int i);
	virtual ~PcbSem();
	virtual int wait(unsigned TimeSlice);
	virtual int signal(int TimeSlice);

	int val;
	int threads_started;
	int r_propusteno;
	int num_of_wait_signals;
	char c;
	int crit;
	virtual void f1(char _c);
	virtual void f();
	virtual void inc();
	virtual void in_crit(int n);
	int write_thread_in_crit;
private:
	friend semQueue;
	friend timeQueue;
	queue* myWaitQueue;
	queue* writeQueue;
	timeQueue* myTimeQueue;

	void block(PCB* thr);
	void unblock(PCB* thr);
};
#endif
