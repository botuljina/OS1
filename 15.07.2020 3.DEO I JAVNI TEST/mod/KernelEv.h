#ifndef _kernelev_h_
#define _kernelev_h_
#include "Event.h"
#include "Pcb.h"
class KernelEv
{
public:
	KernelEv(IVTNo c);
	~KernelEv();

	void signal();
private:
	friend Event;
	friend IVTEntry;
	PCB* kernelPCB;
	IVTNo kernelNo;
	int val;
	void wait();

};

#endif
