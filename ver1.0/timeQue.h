#ifndef HFILES_TIMEQUE_H_
#define HFILES_TIMEQUE_H_
#include "PcbSem.h"
#include "Thread.h"
class PcbSem;
class timeQueue
{
				class qnode
			    {
			      public:
			        PCB* element;
			        Time waitTime;
			        qnode *next;

			        qnode(PCB* e = 0,Time t): element( e ), next( 0 ),waitTime(t)
			        { }
			    };
public:
//standard class operations
	timeQueue();
	~timeQueue();
	int isEmpty();
//queue operations
	void put(PCB* _pcb,Time t);
	PCB* get();
//semaphore operations
	void decrementNodes();
	void printQueueElements();
//public attributes
	PcbSem* myPcbSem;
	int queueLen;
private:
	qnode* firstNode;
	qnode* lastNode;
};
#endif
