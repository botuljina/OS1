#include "semQue.h"
#include "Timer.h"
#include "SCHEDULE.H"
#include <iostream.h>
#include "sPrintf.h"
timeQueue::timeQueue()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	myPcbSem = 0;
	firstNode = 0;
	lastNode = 0;
	queueLen = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void timeQueue::put(PCB* _pcb,Time t)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	if ( isEmpty( ) == 1 )
	  {
	    firstNode = new qnode(_pcb,t);
	    lastNode = firstNode;
	    queueLen++;
	  }
	  else
	  {
	    qnode *p = new qnode(_pcb,t);

	    lastNode->next = p;
	    lastNode = lastNode->next;
	    queueLen++;
	  }
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
PCB* timeQueue::get()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	qnode *temp ;
	PCB* element_for_return;


	if (isEmpty( ) == 0)
		  {
			queueLen--;
		    element_for_return = firstNode->element;
		    temp = firstNode;

		    //pomeram prvi
		    firstNode = firstNode->next;

		    delete temp;
		  }
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
     return element_for_return;

}
timeQueue::~timeQueue()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	while (isEmpty()==0)
	    get();
	firstNode = 0;
	lastNode = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}


//Funkcija: Umanjim val i vadim node kojima je waitTime = 0
void timeQueue::decrementNodes()
{
	//ako je semafor prazan nemoj nista da decrementujes
	if(queueLen==0)
	{
		return;
	}

	qnode* iterator = firstNode;
	while(isEmpty()==0 && (--iterator->waitTime==0))
	{
		//syncPrintf("Prvi elemment ima waitTime =0,decrementNodes() in timeQueue ->prosao 1. if(--previousNode->waitTime==0)\n");
		//cout<<previousNode->waitTime<<endl;
		//pomeram pokazivace
		PCB* el_for_scheduler = get();

		el_for_scheduler->myPcbState = ready;
		Scheduler::put(el_for_scheduler);

		if(myPcbSem!=0) myPcbSem->val++;
		iterator=iterator->next;
	}

	qnode* first =iterator;
	qnode* second = first->next;
	//first je vec umanjen samo se pitas za second
	while(second!=0)
	{
		if(--second->waitTime==0)
		{
			//syncPrintf("->decrementNodes in time queue -> if(--previousNode->waitTime==0) in while loop");
				qnode* temp = second;

				//prevezujem pokazivace
				first->next = second->next;
				second = second->next;


				temp->element->myPcbState = ready;
				Scheduler::put(temp->element);

				if(myPcbSem!=0) myPcbSem->val++;

				queueLen--;

				delete temp;
		}
		else
		{
			first = second;
			second = second->next;
		}

	}
	//za svaki slucaj
	if(queueLen==0)
	{
		firstNode = 0;
		lastNode = 0;
	}
}

void timeQueue::printQueueElements()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	//cout<<"IZBORIO SAM SE SA WHILE PETLJOM KRECE PRINT SVIH ELEMNATA NIZA KOJI TREBA DA BUDE MANJI ZBOG IZBACIVANJA"<<endl;
	qnode* temp = firstNode;
	while(temp!=0)
	{
		//cout<<temp->waitTime<<",<-wait time elementa,quelen timequeue-a -> "<<queueLen<<"  |||";
		temp=temp->next;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
int timeQueue::isEmpty()
{
	if(firstNode==0)return 1;
	else return 0;
}
