#ifndef _ivtentry_h_
#define _ivtentry_h_
#include "Event.h"
#include <DOS.H>
#include "queue.h"

class IVTEntry;
typedef void interrupt(*pRoutine)(...);


extern IVTEntry* IVT[256];


class IVTEntry
{
public:
	IVTEntry(IVTNo c,pRoutine new_routine);
	virtual ~IVTEntry();

	void signal();
	void oldRoutine();

protected:
	friend KernelEv;
	KernelEv* myKernelEv;
	queue* myIVTqueue;
	IVTNo myIVTEntryno;
	pRoutine old_routine;
};


#endif
