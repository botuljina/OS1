#ifndef HFILES_SEMQUE_H_
#define HFILES_SEMQUE_H_
#include "PcbSem.h"

class semQueue
{
				class qnode
			    {
			      public:
			        PcbSem* element;
			        qnode *next;
			        qnode(PcbSem* e = 0): element( e ), next( 0 )
			        { }
			    };
public:
	semQueue();
	~semQueue();

	void put(PcbSem* _sem);
	PcbSem* get();

	void tickAllSemaphores();
	void printQueueElements();
	int isEmpty();

	int queueLen;
private:

	qnode* firstNode;
	qnode* lastNode;
};
#endif
