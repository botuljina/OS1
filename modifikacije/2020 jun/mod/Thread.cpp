#include "Thread.h"
#include "Timer.h"
#include "SCHEDULE.H"
#include "Idle.h"

#include <iostream.h>
#include "sPrintf.h"

ID Thread::getId()
{
	return threadPCB->threadID;
}
/*void Thread::assignIDtoThread()
{
	Timer::globalthreadID++;
    threadID = Timer::globalthreadID;
}*/
//NOTIFIKACIJA: MOJ THREAD KOSTRUKTOR ZA SADA NE STAVLJA U LISTU
Thread::Thread(StackSize s, Time t)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	mypair = 0;
	//stvaram kernel nit
	threadPCB = new PCB(this,s,(volatile unsigned long) t);
	//dodeljujem ID objektu Thread
	//assignIDtoThread();
	//testing
	//if(Timer::testingPhase)
	//	syncPrintf("PRAVIM NIT SA ID-jem %d\n",threadID);
	//implementacija static red
	/*if(Timer::globalQueueForGettingIds==0)
	{
		Timer::globalQueueForGettingIds=new queue();
	}*/
	Timer::globalQueueForGettingIds->put(threadPCB);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

}
Thread::~Thread()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	waitToComplete();
	delete threadPCB;
	mypair = 0;
	in_crit = 0;
	//kada obrisem nit trebalo bih da je izvadim iz global queue-a
	//Timer::globalQueueForGettingIds->getThreadByIdAndRemoveIt(this->getId());
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
//ovaj metod se poziva samo jednom, prilikom startovanja niti
//pre nego sto je stavim u Scheduler moram da uradim sledece:
void Thread::start()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	threadPCB->myPcbState = ready;
	threadPCB->createStack();
	Scheduler::put(threadPCB);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
int Thread::checkIfNeedsWaiting()
{
	if(threadPCB == Timer::idleThread->getIdlePCB())
	{//ako si idle tread ona se nikad ne ceka
		//syncPrintf("uso u petlju 1");
		return 1;
	}
	//ne treba da ceka nit koja se uopste ne nalazi u sheduleru
	if(threadPCB->myPcbState==initialized)
	{
		//syncPrintf("uso u petlju 2");
		return 1;
	}
	//ne ceka se nit koja je zavrsena
	if(threadPCB->myPcbState==finished)
	{
		//syncPrintf("uso u petlju 3");
		return 1;
	}
	//ne ceka na samog sebe
	if(threadPCB == (PCB*)Timer::running)
	{
		//syncPrintf("uso u petlju 4");
		return 1;
	}
	//ako prodjes sve uslove vrati 0
	return 0;
}
void Thread::waitToComplete()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
    // checkIfNeedsWaiting() = 1 -> ne ispunjava uslove za cekanje
	if(checkIfNeedsWaiting()==0)
	{
		Timer::running->myPcbState = blocked;
		threadPCB->PcbQueueForWaitingThreads->put((PCB*)Timer::running);
		Timer::dispatch();
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

}
ID Thread::getRunningId()
{

#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	ID return_val =Timer::running->threadID;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

	return return_val;
}

Thread* Thread::getThreadById(ID id)
{

#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	Thread* thread = Timer::globalQueueForGettingIds->getThreadByIdSearchQueue(id);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return thread;
}

Time minTimeSlice() { return 1; }

//prazna f. simulira timer tick.
//Stavi je pod komentar kad vezujes test


//void tick() {};

//eksplicitni poziv za promenu konteksta
void dispatch()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	Timer::dispatch();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
