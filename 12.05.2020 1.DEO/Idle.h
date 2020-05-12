#ifndef HFILES_IDLE_H_
#define HFILES_IDLE_H_
#include "thread.h"

class Thread;

class Idle:public Thread
{
public:
	friend Timer;
	friend PCB;
	Idle();
	virtual void run();
	void start();
	PCB* getIdlePCB();
};

#endif
