#include "queue.h"
#include "Timer.h"
#include <iostream.h>
#include "sPrintf.h"
#include "Thread.h"
int timeQueue::find_biggest_id()
{
	int biggest = 0;
	qnode* temp = firstNode;
	while ( isEmpty( ) == 0 )
		  {
			if(temp->element->threadID>biggest)
			{
				biggest = temp->element->threadID;
			}
			temp = temp->next;
		  }
	return biggest;
}
PCB* timeQueue::get_by_id(int id)
{
	qnode *cur = firstNode, *prev = 0, *old = 0;
	PCB* el_for_ret = 0;
		while (cur) {
			if (cur->element->threadID != id) {
				prev = cur;
				cur = cur->next;
			} else {
				old = cur;
				cur = cur->next;
				if (!prev)
					firstNode = cur;
				else
					prev->next = cur;
				if(old == lastNode)
					lastNode = prev;
				el_for_ret = old->element;
				delete old;
				queueLen--;
				break;
			}
		}
	return el_for_ret;
}
int queue::find_biggest_id()
{
	int biggest = 0;
	qnode* temp = firstNode;
	while ( temp!= 0 )
	{
			if((int)temp->element->threadID>biggest)
			{
				biggest = (int)temp->element->threadID;
			}
			temp = temp->next;
    }
	return biggest;
}
PCB* queue::get_by_id(int id)
{
	qnode *cur = firstNode, *prev = 0, *old = 0;
	PCB* el_for_ret = 0;
		while (cur) {
			if (cur->element->threadID != id) {
				prev = cur;
				cur = cur->next;
			} else {
				old = cur;
				cur = cur->next;
				if (!prev)
					firstNode = cur;
				else
					prev->next = cur;
				if(old == lastNode)
					lastNode = prev;
				el_for_ret = old->element;
				delete old;
				queueLen--;
				break;
			}
		}
	return el_for_ret;
}
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
/*int queue::getThreadByIdAndRemoveIt(ID id)
{

	qnode* first =firstNode;
	qnode* second = first->next;
	if(first!=0 && first->element->myThread->getId()==id) this->get();
	//first je vec umanjen samo se pitas za second
	while(second!=0)
	{
		if(second->element->myThread->threadID==id)
		{
				qnode* temp = second;

				//prevezujem pokazivace
				first->next = second->next;
				second = second->next;

				queueLen--;

				delete temp;
		}
		else
		{
			first = second;
			second = second->next;
		}

	}
  return -1;
}*/


