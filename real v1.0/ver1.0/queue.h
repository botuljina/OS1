#ifndef HFILES_QUEUE_H_
#define HFILES_QUEUE_H_
#include "Pcb.h"
class PCB;
class queue
{
	   class qnode
	    {
	      public:
	        PCB* element;
	        qnode *next;

	        qnode(PCB* e = 0 ): element( e ), next( 0 )
	        { }
	    };
public:
	friend Timer;
    friend PCB;
	int isEmpty();
	void put(PCB* _pcb);
	PCB* get();
	int getQueueSize();
    void printQueueElements();
	queue();
	~queue();
private:
	qnode* firstNode;
	qnode* lastNode;
	int queueLen;
};

#endif
