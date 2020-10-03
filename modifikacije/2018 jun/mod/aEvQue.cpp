#include "aEvQue.h"
#include "sPrintf.h"

Eventqueue::Eventqueue()
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

int Eventqueue::isEmpty(){

	if(firstNode == 0) return 1;
	else return 0;

}
void Eventqueue::printQueueElements()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	qnode* temp = firstNode;
	syncPrintf("VELICINA queue-a je:%d\n************* \n",queueLen);
	while(temp!=0)
	{
		//if(temp!=0)syncPrintf("%d id niti.\n",(int)temp->element->myThread->getId());
		temp=temp->next;
	}
	syncPrintf("\n************* \n");
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void Eventqueue::put(KernelEv* _kev)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif


	if ( isEmpty( ) == 1 )
	  {
	    firstNode = new qnode(_kev);
	    lastNode = firstNode;
	    queueLen++;
	  }

	  else
	  {
	    qnode *p = new qnode(_kev);

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

KernelEv* Eventqueue::get()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	qnode *temp ;
	KernelEv* element_for_return = 0;

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

Eventqueue::~Eventqueue()
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
int Eventqueue::getQueueSize()
{
	return queueLen;
}

void Eventqueue::my_swap(qnode *node_1, qnode *node_2)
{
	 	KernelEv* temp = node_1->element;
		node_1->element = node_2 -> element;
		node_2 -> element = temp;
}
void Eventqueue::sort()
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
				//u ovoj ovde liniji samo ispravljam da li cu opadajuce ili rastuce
				//if (lPtr->element->myThread->getId() < lPtr->next->element->myThread->getId())
				//{
					my_swap(lPtr, lPtr->next);
	                swapped = 1;
				//}
				lPtr = lPtr->next;
			}
			//as the largest element is at the end of the list, assign that to rPtr as there is no need to
			//check already sorted list
			rPrt = lPtr;

		}while(swapped);
}
//Funkcija: vadi sa odredjene pozicije PCB i uklanja element iz reda
KernelEv* Eventqueue::get_by_id(ID id)
{
	qnode *cur = firstNode, *prev = 0, *old = 0;
	while (cur) {
		//cur->element->myThread->getId() != id
		//ovaj deo se ispravlja: recimo if(cur->element->ev_priority()==id);
		if (1) {
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

			delete old;
			queueLen--;
			break;
		}
	}
}
