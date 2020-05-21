#ifndef HFILES_TIMER_H_
#define HFILES_TIMER_H_

//makroi koji obezbedjuju atomicnost (koristimo pushf i popf
//zbog gnezdenja poziva vise atomicnih f-ja na steku da ne bi
//doslo do stvaranja "otkljucanih" delova koda
//PUSHF
//1. SP = SP - 2
//2. SS:[SP] (top of the stack) = flags
#define lock() {asm{pushf; cli;}}
//POPF
//1. flags = SS:[SP] (top of the stack)
//2. SP = SP + 2
//vracamo PSW (i bit I) na stanje pre zakljucavanja
#define unlock() {asm{popf;}}
#include "semQue.h"

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
	friend PCB;
	friend Thread;
	friend Idle;
	friend queue;
	friend PcbSem;

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
