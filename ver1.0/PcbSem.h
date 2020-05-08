
#ifndef HFILES_PCBSEM_H_
#define HFILES_PCBSEM_H_
#include "queue.h"
#include "timeQue.h"

class PcbSem{
public:
	PcbSem(int i);
	virtual ~PcbSem();

	virtual int wait(int TimeSlice);
	virtual int signal(int TimeSlice);

	int val;
private:
	friend semQueue;
	friend timeQueue;
	queue* myWaitQueue;
	timeQueue* myTimeQueue;

	void block(PCB* thr);
	void unblock(PCB* thr);
};
#endif
