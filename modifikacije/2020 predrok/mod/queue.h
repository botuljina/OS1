#ifndef HFILES_QUEUE_H_
#define HFILES_QUEUE_H_
#include "Pcb.h"
class Thread;
typedef unsigned long int ID;
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
    Thread* getThreadByIdSearchQueue(ID id);
    //int getThreadByIdAndRemoveIt(ID id);
	queue();
	~queue();
	 int find_biggest_id();
	 PCB* get_by_id(int id);
	//modifikacija extenzija


private:
	qnode* firstNode;
	qnode* lastNode;
	int queueLen;
};

#endif
