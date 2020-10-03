#include "Event.h"
#include "KernelEv.h"
#include "Timer.h"
#include "sPrintf.h"

void Event::reserve(KeyboardEvent* k)
{
	this->reservedEvents->put(k->threadPCB);
}
Event::Event(IVTNo ivtNo)
{
	syncPrintf("Kreiram event\n");
	this->myKernelev=new KernelEv(ivtNo);
	reservedEvents = new queue();
}

Event::~Event()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	delete this->myKernelev;
	delete reservedEvents;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void Event::wait()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	if(reservedEvents->getThreadByIdSearchQueue(Timer::running->myThread->getId()))
	this->myKernelev->wait();
	else
	{
		//syncPrintf("ne blokira se %d\n", (int)Timer::running->myThread->getId());
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void Event::signal()
{
	this->myKernelev->signal();
}
