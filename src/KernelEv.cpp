#include "KernelEv.h"
#include "Timer.h"
#include "SCHEDULE.H"
#include "IVTEntry.h"
KernelEv::KernelEv(IVTNo c)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	kernelPCB = (PCB*)Timer::running;
	kernelNo = c;
	val = 0;
	// pazi!
	IVT[this->kernelNo]->myKernelEv=this;

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
KernelEv::~KernelEv()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	kernelPCB=0;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void KernelEv::wait()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	if(Timer::running->myThread->getId()== kernelPCB->myThread->getId())
	{
		if(val==1) val=0 ;
		else if(val==1)
		{
			kernelPCB->myPcbState= blocked;
			#ifndef BCC_BLOCK_IGNORE
			unlock();
			#endif
			Timer::dispatch();
			return;
		}
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void KernelEv::signal()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	//Ako je pokaziva na blokiranu nit jednak 0,moras postaviti value na 1.
	if(kernelPCB->myPcbState==blocked)
	{//ako je nit blokirana, signalom je odblokiraj i put u scheduler
		this->kernelPCB->myPcbState=ready;
		Scheduler::put(kernelPCB);
	}
	else
	{
		val=1;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
