#include "semQue.h"
#include "Timer.h"
#include <IOSTREAM.H>
#include "sPrintf.h"
semQueue::semQueue()
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

semQueue::semQueue(int i)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	syncPrintf("OVO POZIVA SAMO STATIC SEM QUEUE\n");
	firstNode = 0;
	lastNode  = 0;
	queueLen = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

semQueue::~semQueue()
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

void semQueue::put(PcbSem* _sem)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	/*
	if(this==0){
		Timer::mySemQueue = new semQueue();
		Timer::mySemQueue->put(_sem);
		return;
	}*/
	int i = isEmpty();
	//syncPrintf("sem koji se dodaje je -> %p\n.Vrednost isEmpty() ->%d\n",_sem,i);
	//syncPrintf("firstNode -> %p",firstNode);

	syncPrintf("QUEUELEN je-> %d\n",queueLen);
	if (firstNode == 0 )
	  {
		syncPrintf("sem koji se dodaje je BATONI MOJ-> %p",_sem);
	    firstNode = new qnode(_sem);
	    lastNode = firstNode;
	    queueLen++;
	  }

	  else
	  {
	    qnode *p = new qnode(_sem);

	    lastNode->next = p;
	    lastNode = lastNode->next;
	    queueLen++;
	  }
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

PcbSem* semQueue::get()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	qnode *temp ;
	PcbSem* element_for_return;

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
void semQueue::tickAllSemaphores()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	qnode* temp = firstNode;
	int i=0;
	//syncPrintf("VELICINA SEM QUEUE-a je %d",queueLen);
	while(temp!=0)
	{
		//cout<<i++<<"->VELICINA SEMQUEUE->"<<queueLen<<"->temp->"<<temp<<endl;
		temp->element->myTimeQueue->decrementNodes();
		temp = temp->next;
		//cout<<"temp->"<<temp<<endl;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
void semQueue::printQueueElements()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	cout<<"krenuo printAll"<<endl;
	qnode* temp = firstNode;
	int iter=0;
	while(temp!=0)
	{
		cout<<iter++<<"->"<<temp->element<<"semqueue adresa->"<<this<<"\n";
		temp = temp->next;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
int semQueue::isEmpty()
{
	if(firstNode==0)return 1;
	else return 0;
}
