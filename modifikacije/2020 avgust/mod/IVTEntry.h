#ifndef _ivtentry_h_
#define _ivtentry_h_
#include "Event.h"
#include <DOS.H>
#include "aEvQue.h"
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
	Eventqueue* queue;
protected:
	friend KernelEv;
	KernelEv* myKernelEv;

	IVTNo myIVTEntryno;
	pRoutine old_routine;
};


#endif
