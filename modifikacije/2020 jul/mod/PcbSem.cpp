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

	Timer::mySemQueue->put(this);
	//moram da referenciram Semaphor, kako bih uvecavao val vrednost istog
	myTimeQueue->myPcbSem = this;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

int PcbSem::wait(unsigned TimeSlice)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
    val = val-1;
   // syncPrintf("wait() val--, val =%d !\n",val);
    Timer::running->SemRetVal = 1;
    if(val<0)
    {
    	PCB* toBlock = (PCB*)Timer::running;
    	block(toBlock);

    	if(TimeSlice>0)
    	{//nit se blokira za int TimeSlice,nit stavljam u TimeQueue
    		myTimeQueue->put(toBlock,TimeSlice);
    		if(Timer::testingPhase)
    		    				syncPrintf("UBACUJEM U TIME QUEUE! %p\n",this);
    	}
    	else
    	{
    		if(TimeSlice==0)
    		{
    			syncPrintf("NIT->%d se **blokira** na eventu\n",(int)toBlock->myThread->getId());
    			if(Timer::testingPhase)
    				syncPrintf("UBACUJEM U **OBICAN**! %p\n",this);
    			myWaitQueue->put(toBlock);
    		}
    	}
    	//EKSPLICITNI POZIV DISPATCH FUNKCIJE
    	Timer::dispatch();
    }else
    {
    	syncPrintf("NIT->%d se propusta na eventu\n",(int)Timer::running->myThread->getId());
    }
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return Timer::running->SemRetVal;
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


    maxThreadstoUnblock = val*(-1);

    //ovo je najbitnija linija, znaci moram da uvecam val atribut za vrednost signalValue,
    //vrednost je malo modifikovana prethodnim if-om
    val += maxThreadstoUnblock;
    //syncPrintf("signal(), val+=maxThreadstoUnblovk val =%d\n",val);
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
    			 int stop = 1;
    			 while(stop)
    			 {
    				 PCB* toUnblock = myWaitQueue->get();
    				 if(toUnblock==0){
    					 stop = 0;
    					 break;
    				 }
    				 unblock(toUnblock);
    				  syncPrintf("<<Odblokiram>> nit -> %d\n",(int)toUnblock->myThread->getId());
    				  toUnblock->SemRetVal=0;
    				  Scheduler::put(toUnblock);
    				  shouldGetFromTimeQueue++;
    				  returnVal++;
    			 }

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

