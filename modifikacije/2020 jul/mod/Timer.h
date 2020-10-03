#ifndef HFILES_TIMER_H_
#define HFILES_TIMER_H_
#include "semQue.h"

#define lock() {asm{pushf; cli;}}
#define unlock() {asm{popf;}}
class queue;
class Timer {
public:
	Timer();

	static void start_system_initialization();
	static void SetAllPrefiniedAtrributes();
	static void restore_system();
	static void dispatch();


	virtual ~Timer();
	static unsigned long globalthreadID;
	static queue* globalQueueForGettingIds;
	static semQueue* mySemQueue;

private:
	friend Event;
	friend BoundedBuffer;
	friend PCB;
	friend Thread;
	friend Idle;
	friend queue;
	friend PcbSem;
	friend KernelEv;

	static int testingPhase;
	static Thread* startingThread;
	static Idle* idleThread;
	static volatile PCB* running;
	static volatile unsigned dispatched;

	//pointer to function with no arguments
	static void interrupt (*oldRoutine) (...);
	static void interrupt timerRoutine(...);
	static void wrapper();

};

#endif /* HFILES_TIMER_H_ */
