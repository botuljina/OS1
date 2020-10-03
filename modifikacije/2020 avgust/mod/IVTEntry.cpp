#include "IVTEntry.h"
#include "KernelEv.h"
#include "Timer.h"

IVTEntry* IVT[256]={0};


IVTEntry::IVTEntry(IVTNo c,pRoutine new_routine)
{
	myIVTEntryno = c;
	old_routine=0;
	myKernelEv = 0;
	IVT[myIVTEntryno] = this;
	this->queue = new Eventqueue();
#ifndef BCC_BLOCK_IGNORE
	old_routine=getvect(myIVTEntryno);
	setvect(myIVTEntryno,new_routine);
#endif
}

IVTEntry::~IVTEntry()
{
	this->myKernelEv=0;
	IVT[myIVTEntryno]=0;

	oldRoutine();
#ifndef BCC_BLOCK_IGNORE
	setvect(myIVTEntryno,old_routine);
#endif
}

void IVTEntry::signal()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	queue->signalAll();
	//myKernelEv->signal();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void IVTEntry::oldRoutine()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	old_routine();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
