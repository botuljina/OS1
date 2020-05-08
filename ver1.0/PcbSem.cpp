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
	syncPrintf("POZVAN SEM KONSTRUKTOR -> %p\n",this);
	if(i<0) val =0 ;
	else val = i;

	myTimeQueue = new timeQueue();
		myWaitQueue = new queue();

	if(Timer::mySemQueue!=0)
		Timer::mySemQueue->put(this);
	else
	{
		Timer::mySemQueue = new semQueue();
		Timer::mySemQueue->put(this);
	}


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
	//cout<<"ULAZI U WAIT"<<endl;
	int returnVal=1;
    val = val -1;
    if(val<0)
    {
    	PCB* toBlock = (PCB*)Timer::running;
    	block(toBlock);
    	if(TimeSlice>0)
    	{//nit se blokira za int TimeSlice
    		myTimeQueue->put(toBlock,TimeSlice);
    		//syncPrintf("printam %p toblock\n",toBlock);
    		//syncPrintf("printam %p starting\n",Timer::startingThread->threadPCB);
    		//myTimeQueue->printQueueElements();
    	}
    	else
    	{
    		if(TimeSlice==0)
    		{

    			myWaitQueue->put(toBlock);
    		}
    	}

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

    int returnVal = 0;
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
    int maxThreadstoUnblock = 0;


    if(signalValue==0) maxThreadstoUnblock =1;
    else maxThreadstoUnblock = signalValue;

    //ovo je najbitnija linija, znaci moram da uvecam val atribut za vrednost signalValue,
    //vrednost je malo modifikovana prethodnim if-om
    val += maxThreadstoUnblock;

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
    {//ostalo je jos elemenata, koji wi trebalo iz drugog reda sad da se izvade
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

