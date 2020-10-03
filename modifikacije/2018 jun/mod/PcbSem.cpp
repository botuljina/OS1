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
	threads_in_critical = new queue();
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
    //val = val-1;
    Timer::running->SemRetVal = 1;
    if(--val<0 || Timer::running->friends_queue->check(this->threads_in_critical)==1)
    {

    	PCB* toBlock = (PCB*)Timer::running;
    	block(toBlock);
    	syncPrintf("Blokiram nit %d\n",(int)Timer::running->myThread->getId());
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
    			if(Timer::testingPhase)
    				syncPrintf("UBACUJEM U **OBICAN**! %p\n",this);
    			myWaitQueue->put(toBlock);
    		}
    	}
    	//EKSPLICITNI POZIV DISPATCH FUNKCIJE
    	Timer::dispatch();
    }else
    {
    	syncPrintf("Prosla nit: %d, val = %d\n",(int)Timer::running->myThread->getId(),val);
    	this->threads_in_critical->put((PCB*)Timer::running);
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
	this->threads_in_critical->get_by_id((int)Timer::running->myThread->getId());
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
    			 //proveri da li red sadrzi blokiranu nit koja je moj friend
    			 if(myWaitQueue->hasBlocked(Timer::running->friends_queue)!=0)
    			 {
    				 //syncPrintf("ODBLOKIRAM %d\n",myWaitQueue->hasBlocked(Timer::running->friends_queue));
    				 PCB* toUnblock = myWaitQueue->get_by_id((ID)myWaitQueue->hasBlocked(Timer::running->friends_queue));
    				 syncPrintf("SIGNAL, odblokiram **FRENDA**->%d\n", (int)toUnblock->myThread->getId());
    				 unblock(toUnblock);
    				 toUnblock->SemRetVal=0;
    				 Scheduler::put(toUnblock);
    				 shouldGetFromTimeQueue++;
    				 returnVal++;
    			 }
    			 else
    			 {
    				 if(threads_in_critical->queueLen==0)
    				 {
    					 PCB* toUnblock = myWaitQueue->get();
    					 unblock(toUnblock);
    					 toUnblock->SemRetVal=0;
    					 Scheduler::put(toUnblock);
    					 shouldGetFromTimeQueue++;
    					 returnVal++;
    					  syncPrintf("SIGNAL, odblokiram nesto drugo jer sam poslednji u criticalu->%d\n", (int)toUnblock->myThread->getId());
    				 }

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
	delete threads_in_critical;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

