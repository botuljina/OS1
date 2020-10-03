#ifndef _event_h_
#define _event_h_
typedef unsigned char IVTNo;
class KernelEv;
#include "thread.h"
#include "IVTEntry.h"
#include "keyevent.h"
#define PREPAREENTRY(No,flag)\
void interrupt intr##No(...);\
IVTEntry ivte##No(No,intr##No);\
void interrupt intr##No(...){\
	ivte##No.signal();\
	if(flag==1){\
		ivte##No.oldRoutine();\
	}\
	dispatch();\
}

class Event {
public:
	friend KeyboardEvent;
	friend PCB;
	friend Thread;
	Event(IVTNo ivtNo);
	~Event();
	void wait();
	queue* reservedEvents;
	void reserve(KeyboardEvent* k);
protected:
	friend class KernelEv;
	void signal();
private:
	KernelEv* myKernelev;

};

#endif
