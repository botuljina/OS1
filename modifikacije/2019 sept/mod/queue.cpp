#include "queue.h"
#include "Timer.h"
#include <iostream.h>
#include "sPrintf.h"
#include "Thread.h"

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
	syncPrintf("VELICINA queue-a je:%d\n************* \n",queueLen);
	while(temp!=0)
	{
		//if(temp!=0)syncPrintf("%d id niti.\n",temp->element->myThread->threadID);
		temp=temp->next;
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
	if(Timer::testingPhase)
	{
		if(this!= Timer::globalQueueForGettingIds) syncPrintf("Ubacivanje u neki drugi queue,id=%d,addr:%p,\n",_pcb->myThread->getId());
		else syncPrintf("Ubacivanje u gloabal queue,id=%d\n",_pcb->myThread->getId());
	}

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

	if(Timer::testingPhase)
				printQueueElements();

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
	PCB* element_for_return = 0;



	if ( isEmpty( ) == 0 )
		  {
			queueLen--;
		    element_for_return = firstNode->element;
		    temp = firstNode;

		    //pomeram prvi
		    firstNode = firstNode->next;

		    delete temp;
		  }
	if(Timer::testingPhase)
	{
		if(this!= Timer::globalQueueForGettingIds) syncPrintf("POP iz drugog queue-a,id=%d, addr:%p,\n",element_for_return->myThread->getId());
		else syncPrintf("POP iz GLOBAL queue-a,id=%d\n",element_for_return->myThread->getId());

		printQueueElements();
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
Thread* queue::getThreadByIdSearchQueue(ID id)
{
	qnode *temp = firstNode;
	while(temp!=0)
	{
		if(temp->element->myThread->getId() == id)
		{
			return temp->element->myThread;
		}
		temp=temp->next;
	}
	return 0;
}
void queue::my_swap(qnode *node_1, qnode *node_2)
{
	 	PCB* temp = node_1->element;
		node_1->element = node_2 -> element;
		node_2 -> element = temp;
}
void queue::sort()
{
	int swapped;

	    qnode *lPtr; // left pointer will always point to the start of the list
		qnode *rPrt = NULL; // right pointer will always point to the end of the list

		do
		{
			swapped = 0;
			lPtr = firstNode;
			while(lPtr->next != rPrt)
			{
				if (lPtr->element->myThread->getId() > lPtr->next->element->myThread->getId())
				{
					my_swap(lPtr, lPtr->next);
	                swapped = 1;
				}
				lPtr = lPtr->next;
			}
			//as the largest element is at the end of the list, assign that to rPtr as there is no need to
			//check already sorted list
			rPrt = lPtr;

		}while(swapped);
}
