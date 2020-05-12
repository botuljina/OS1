#include "Thread.h"
#include "Timer.h"
#include "SCHEDULE.H"
#include "Idle.h"

#include <iostream.h>
#include "sPrintf.h"

ID Thread::getId()
{
	return threadID;
}
void Thread::assignIDtoThread()
{
	Timer::globalthreadID++;
    threadID = Timer::globalthreadID;
}
//NOTIFIKACIJA: MOJ THREAD KOSTRUKTOR ZA SADA NE STAVLJA U LISTU
Thread::Thread(StackSize s, Time t)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	//proveriti da li mi ovo radi dobar cast

	threadPCB = new PCB(this,s,(volatile unsigned long) t);
	assignIDtoThread();
	if(Timer::testingPhase)
		syncPrintf("PRAVIM NIT SA ID-jem %d\n",threadID);

	if(Timer::globalQueueForGettingIds==0)
	{
		Timer::globalQueueForGettingIds=new queue();
	}
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
	//waitToComplete();
	delete threadPCB;
	//kada obrisem nit trebalo bih da je izvadim iz global queue-a
	Timer::globalQueueForGettingIds->getThreadByIdAndRemoveIt(this->getId());
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
	//cout<<"steka treda je: "<< threadPCB->stack <<"\n";
	Scheduler::put(threadPCB);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
int Thread::checkIfNeedsWaiting()
{
	if(threadPCB == Timer::idleThread->getIdlePCB())
		{
		//syncPrintf("uso u petlju 1");
		//while(1);
		return 1;

		}
	//ne treba da ceka nit koja se uopste ne nalazi u sheduleru
	if(threadPCB->myPcbState==initialized)
		{
		//syncPrintf("uso u petlju 2");
				//while(1);
		return 1;

		}
	//ne ceka se nit koja je zavrsena
	if(threadPCB->myPcbState==finished)
	{
			//syncPrintf("uso u petlju 3");
					//while(1);
			return 1;

			}
	//ne ceka na samog sebe
	if(threadPCB == (PCB*)Timer::running){
		{
				//syncPrintf("uso u petlju 4");
						//while(1);
				return 1;

				}
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
		//ne razumem zasto ga ne pusta da pozove put bez castovanja
		//iz razloga sto je running svakako pcb*
		threadPCB->PcbQueueForWaitingThreads->put((PCB*)Timer::running);


		dispatch();
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

}
ID Thread::getRunningId()
{
	return Timer::running->myThread->threadID;
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
