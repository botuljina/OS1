#include "queue.h"
#include "Timer.h"
#include <iostream.h>

queue::queue()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	firstNode = 0;
	lastNode  = 0;
	queueLen = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

int queue::isEmpty(){

	if(firstNode == 0) return 1;
	else return 0;

}
void queue::printQueueElements()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	qnode* temp = firstNode;
	while(temp!=0)
	{
		//syncPrintf("%d",temp->element);
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void queue::put(PCB* _pcb)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	if ( isEmpty( ) == 1 )
	  {
	    firstNode = new qnode(_pcb);
	    lastNode = firstNode;
	    queueLen++;
	  }

	  else
	  {
	    qnode *p = new qnode(_pcb);

	    lastNode->next = p;
	    lastNode = lastNode->next;
	    queueLen++;
	  }
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

PCB* queue::get()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	qnode *temp ;
	PCB* element_for_return;


	if ( isEmpty( ) == 0 )
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

queue::~queue()
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
int queue::getQueueSize()
{
	return queueLen;
}

//OBAVEZNO PROVERI DA LI SI ISPRAVNO NAPRAVIO RED
