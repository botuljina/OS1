//********
#ifndef HFILES_AEVQUE_H_
#define HFILES_AEVQUE_H_
//********
#include "Pcb.h"
#include "Timer.h"
class Thread;
typedef unsigned long int ID;

class KernelEv;

class Eventqueue
{
	   class qnode
	    {
	      public:
		    //********
		    KernelEv* element;
	        //********
	        qnode *next;

	        qnode(KernelEv* e = 0 ): element( e ), next( 0 )
	        { }

	    };
public:
	friend Timer;
    friend PCB;

	int isEmpty();
	void put(KernelEv* _pcb);
	//********
	KernelEv* get();
	//********
	int getQueueSize();
    void printQueueElements();

    Eventqueue();
	~Eventqueue();
	//--------------------------------------------------
	//optimizacione metode:
	void sort();
	void my_swap (qnode *node_1, qnode *node_2);

	KernelEv* get_by_id(ID id);
private:
	qnode* firstNode;
	qnode* lastNode;
	int queueLen;
};

#endif
