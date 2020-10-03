#include "Event.h"
#include "KernelEv.h"
#include "Timer.h"

Event::Event(IVTNo ivtNo)
{
	this->myKernelev=new KernelEv(ivtNo);
}

Event::~Event()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	delete this->myKernelev;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void Event::wait()
{
	this->myKernelev->wait();
}

void Event::signal()
{
	this->myKernelev->signal();
}
