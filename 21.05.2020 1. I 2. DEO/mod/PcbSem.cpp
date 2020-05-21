#include "timeQue.h"
#include "Timer.h"
#include "SCHEDULE.H"
#include "Idle.h"
#include "sPrintf.h"
#include <iostream.h>

class timeQueue;

PcbSem::PcbSem(int i)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	if(Timer::testingPhase)
		syncPrintf("POZVAN SEM KONSTRUKTOR -> %p\n",this);

	if(i<0) val =0 ;
	else val = i;

	if(Timer::testingPhase)
		syncPrintf("****val-> %d\n",val);

	myTimeQueue = new timeQueue();
	myWaitQueue = new queue();

	if(Timer::mySemQueue==0)
	{
		Timer::mySemQueue = new semQueue();
	}
	Timer::mySemQueue->put(this);
	//moram da referenciram Semaphor, kako bih uvecavao val vrednost istog
	myTimeQueue->myPcbSem = this;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

int PcbSem::wait(int TimeSlice)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	int returnVal=1;
    val = val -1;
    if(val<0)
    {
    	PCB* toBlock = (PCB*)Timer::running;
    	block(toBlock);
    	if(TimeSlice>0)
    	{//nit se blokira za int TimeSlice,nit stavljam u TimeQueue
    		myTimeQueue->put(toBlock,TimeSlice);
    		//syncPrintf("printam %p toblock\n",toBlock);
    		//syncPrintf("printam %p starting\n",Timer::startingThread->threadPCB);
    		//myTimeQueue->printQueueElements();
    	}
    	else
    	{
    		if(TimeSlice==0)
    		{
    			if(Timer::testingPhase)
    				syncPrintf("UBACUJEM U OBICAN WAIT QUEUE");
    			myWaitQueue->put(toBlock);
    		}
    	}

    	//EKSPLICITNI POZIV DISPATCH FUNKCIJE
    	Timer::dispatch();

    	if (toBlock->SemRetVal==1) {
    	            toBlock->SemRetVal=0;
    	            returnVal = 0;
    	        }
    }
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	//int ret = Timer::running->SemRetVal;
	return returnVal;
}

int PcbSem::signal(int signalValue) {
    if (signalValue<0) return signalValue;
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	int returnVal = 0;
    int maxThreadstoUnblock = 0;

    if(signalValue==0) maxThreadstoUnblock =1;
    else maxThreadstoUnblock = signalValue;

    //ovo je najbitnija linija, znaci moram da uvecam val atribut za vrednost signalValue,
    //vrednost je malo modifikovana prethodnim if-om
    val += maxThreadstoUnblock;
    //INDIKATOR koliko sam niti odblokirao iz waitqueue-a,kako bih znao
    //koliko je niti potrebno odblokirati iz timequeue-a
    int shouldGetFromTimeQueue = 0;
    //ukoliko wait queue ima pokazivac na FirstNode, udji u petlju!
    if(myWaitQueue->isEmpty()==0)
    {
    	 for(int i=0;i<maxThreadstoUnblock;i++)
    	 {
    		 if(myWaitQueue->isEmpty()==0)
    		 {
    			 PCB* toUnblock = myWaitQueue->get();
    			 unblock(toUnblock);
    			 toUnblock->SemRetVal=0;
    			 Scheduler::put(toUnblock);
    			 shouldGetFromTimeQueue++;
    			 returnVal++;
    		 }
    	 }
    }
    if(shouldGetFromTimeQueue<signalValue)
    {//ostalo je jos elemenata, koji bi trebalo iz drugog reda sad da se izvade
    	int difference = signalValue - shouldGetFromTimeQueue;
    	for(int i=0;i<difference;i++)
    	{
    		 if(myTimeQueue->isEmpty()==0)
    		   {
    		    	PCB* toUnblock = myWaitQueue->get();
    		    	toUnblock->SemRetVal=1;
    		    	unblock(toUnblock);
    		    	Scheduler::put(toUnblock);
    		    	returnVal++;
    		    }
    	}
    }
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
    return returnVal;
}

void PcbSem::block(PCB *thr)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	thr->myPcbState = blocked;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void PcbSem::unblock(PCB* thr)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	thr->myPcbState = ready;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

PcbSem::~PcbSem()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	delete myWaitQueue;
	delete myTimeQueue;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

