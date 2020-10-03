
#ifndef HFILES_PCBSEM_H_
#define HFILES_PCBSEM_H_
#include "queue.h"
#include "timeQue.h"

class PcbSem{
public:
	PcbSem(int i);
	virtual ~PcbSem();
	virtual int wait(unsigned TimeSlice);
	virtual int signal(int TimeSlice);
	void set_priority(int val);
	int val;
	int priority;
private:
	friend semQueue;
	friend timeQueue;
	queue* myWaitQueue;
	timeQueue* myTimeQueue;

	void block(PCB* thr);
	void unblock(PCB* thr);
};
#endif
