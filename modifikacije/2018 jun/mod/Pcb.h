//File: PCB.h
#ifndef HFILES_PCB_H_
#define HFILES_PCB_H_
#include "queue.h"

class Thread;
class queue;

typedef unsigned long int ID;


typedef unsigned long StackSize;
enum pcbState {
	initialized,
	ready,
	finished,
	blocked
};

class queue;

class PCB
{
public:
	PCB(Thread* t,StackSize s,volatile unsigned long timeSlice);
	void setDefaultValuesForAllAttributes();

	~PCB();

	queue* PcbQueueForWaitingThreads;
	int SemRetVal;

	void createStack();
	queue* friends_queue;
private:
	friend Thread;
	friend Idle;
	friend Timer;
	friend queue;
	friend PcbSem;
	friend timeQueue;
	friend KernelEv;
//atribute sam inicijalizovao u pozivu metode createStack()
	StackSize stackSize;
	unsigned* stack;
	unsigned ss;
	unsigned sp;
	unsigned bp;
//---------------------------------------------------------
//atribute sam inicijalizovao u pozivu PCB konstruktora
	volatile pcbState myPcbState;
	volatile unsigned long TimePassedForThreadPerforming;
	volatile unsigned long PcbTimeSlice;
	Thread* myThread;

	ID threadID;
};

#endif
