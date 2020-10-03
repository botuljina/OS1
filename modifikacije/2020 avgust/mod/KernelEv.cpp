#include "KernelEv.h"
#include "Timer.h"
#include "SCHEDULE.H"
#include "IVTEntry.h"
#include "sPrintf.h"
#include <stdlib.h>
KernelEv::KernelEv(IVTNo c):sem(0)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	kernelPCB = (PCB*)Timer::running;
	kernelNo = c;
	//val = 0;
	// pazi!
	//sem = new Semaphore(0);
	//IVT[this->kernelNo]->myKernelEv=this;
	this->prior = rand() % 5 + 1;
	IVT[this->kernelNo]->queue->put(this);
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
	/*
	if(Timer::running->myThread->getId()== kernelPCB->myThread->getId())
	{
		if(val==1)
		{
			//syncPrintf("WAIT PROLAZ 1\n");
			val=0 ;
		}
		else if(val==0)
		{
			//syncPrintf("WAIT PROLAZ 2\n");
			kernelPCB->myPcbState= blocked;
			#ifndef BCC_BLOCK_IGNORE
			unlock();
			#endif
			Timer::dispatch();
			return;
		}

	}*/
	if (kernelPCB == Timer::running)
	{
		syncPrintf("prior->%d  ",prior);
		sem.wait(0);
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
	if (sem.val()<0)
	{
		//syncPrintf("RADIM SIGNAL");
		sem.signal();
	}

	/*
	//Ako je pokaziva na blokiranu nit jednak 0,moras postaviti value na 1.
	if(kernelPCB->myPcbState==blocked)
	{//ako je nit blokirana, signalom je odblokiraj i put u scheduler
		this->kernelPCB->myPcbState=ready;
		Scheduler::put(kernelPCB);
	}
	else
	{
		val=1;
	}*/
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
