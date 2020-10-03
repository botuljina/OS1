#ifndef _kernelev_h_
#define _kernelev_h_
#include "Event.h"
#include "Pcb.h"
#include "Semaphor.h"
class KernelEv
{
public:
	KernelEv(IVTNo c);
	~KernelEv();

	void signal();
	void wait();
private:
	friend Event;
	friend IVTEntry;

	PCB* kernelPCB;
	IVTNo kernelNo;
	Semaphore sem;

};

#endif
