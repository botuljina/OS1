#include "timeQue.h"
#include "Timer.h"
#include "SCHEDULE.H"
#include "Idle.h"
#include "sPrintf.h"
#include <iostream.h>

class timeQueue;
void PcbSem::in_crit(int n)
{
	this->crit = n;
}
void PcbSem::inc()
{
	this->threads_started++;
}
void PcbSem::f1(char _c)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	this->c = _c;
	this->num_of_wait_signals++;
	if(Timer::running->myThread->c =='r'){
		  if(this->crit == 0)
		  {
			  syncPrintf("Propustam nit sa id: %d zato sto je 'r' \n",Timer::running->myThread->getId());

			  			r_propusteno ++;
		  }
		  else {
			  syncPrintf("Stavljam u red blokiranih na semaforu, nit sa id: %d koja je 'r' \n\n",Timer::running->myThread->getId());
			  PCB* toBlock = (PCB*)Timer::running;
			  block(toBlock);
			  myWaitQueue->put(toBlock);
			  Timer::dispatch();
		  }

	}
	if(Timer::running->myThread->c == 'w')
	{
		if(r_propusteno == 0 && crit ==0)
		{
			syncPrintf("Propustam write ,nit sa id: %d zato sto je 'w' !! \n",Timer::running->myThread->getId());
		}
		else
		{
			syncPrintf("Stavljam u red blokiranih na semaforu, nit sa id: %d zato sto je 'w' \n\n",Timer::running->myThread->getId());
						PCB* toBlock = (PCB*)Timer::running;
						block(toBlock);
						myWaitQueue->put(toBlock);
						Timer::dispatch();
		}


	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
void PcbSem::f()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	if(r_propusteno >0)
	{
		syncPrintf("IDE poziv funkcije f(), r_propusteno = %d\n",r_propusteno);
		r_propusteno--;
		if(r_propusteno==0 && num_of_wait_signals==threads_started)
		{
			syncPrintf("********WADIM IZ QUEUE_A U MOMENTU KAD JE r_propusteno = 0*******\n""");
			this->signal(1);
		}
	}else
	{
		//nema ni jedne propustene 'r' niti signaliziraj za odblok 1
		this->signal(1);
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

PcbSem::PcbSem(int i)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	this->c ='n';
	this->r_propusteno = 0;
	this->threads_started = 0;
	this->num_of_wait_signals = 0;
	this->crit = 0;
	if(Timer::testingPhase)
		syncPrintf("POZVAN SEM KONSTRUKTOR -> %p\n",this);

	if(i<0) val =0 ;
	else val = i;

	if(Timer::testingPhase)
		syncPrintf("****val-> %d\n",val);

	syncPrintf("POZVAN SEM KONSTRUKTOR -> %d\n",val);
	myTimeQueue = new timeQueue();
	myWaitQueue = new queue();


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
	    Timer::running->SemRetVal = 1;
	    if(val<0)
	    {

	    	PCB* toBlock = (PCB*)Timer::running;
	    	block(toBlock);
	    	//syncPrintf("Blokiram na semaforu nit->%d **\n",toBlock->myThread->getId());
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
	    	//syncPrintf("PROPUSTAM na semaforu nit->%d ** sa 'r' pristupom\n",(int)Timer::running->myThread->getId());
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
    			 if(myWaitQueue->getThreadByChar('r')!=0)
    			 { //vadi sve niti koje su r
    				 syncPrintf("ULAZ 1 signal u if");
    				 while(myWaitQueue->getThreadByChar('r')!=0)
    				 { //kaze da postoji
    					 PCB* toUnblock = myWaitQueue->getThreadByChar('r')->threadPCB;
    					 if(myWaitQueue->removeThread(toUnblock->myThread->getId())==1)
    					 {
    						 syncPrintf("Vadim nit sa read pristupom iz reda\n");
    					 }else
    					 {
    						 syncPrintf("QUEUE ERROR EXCEPTION DAFUQ\n");
    					 }
    					 unblock(toUnblock);
    					 toUnblock->SemRetVal=0;
    				     syncPrintf("SIGNAL ALL ZA R na semaforu nit->%d ide u Scheduler, sa pravom pristupa %c\n",(int)toUnblock->myThread->getId(),toUnblock->myThread->c);
    					 Scheduler::put(toUnblock);
    					 this->r_propusteno++;
    					 shouldGetFromTimeQueue++;
    					 returnVal++;

    				 }
    			 }
    			 else
    			 {
    				 syncPrintf("ULAZ 2 signal u if");
    				 PCB* toUnblock = myWaitQueue->get();
    				     			 unblock(toUnblock);
    				     			 toUnblock->SemRetVal=0;
    				     			 syncPrintf("SIGNAL na semaforu nit->%d ide u Scheduler\n",toUnblock->myThread->getId());
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

