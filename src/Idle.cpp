#include "Idle.h"
#include "Timer.h"
#include <iostream.h>
void Idle::run()
{
	while(1)
	{

	}
}

void Idle::start()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	threadPCB->myPcbState = ready;
	threadPCB->createStack();
	//cout<<"steka Idle treda je: "<< threadPCB->stack <<"\n";
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

Idle::Idle():Thread(4096,1)
{
}
PCB* Idle::getIdlePCB()
{
	return threadPCB;
}
