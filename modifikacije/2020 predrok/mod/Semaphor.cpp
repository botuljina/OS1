#include "Semaphor.h"
#include "Timer.h"
#include <IOSTREAM.H>

Semaphore::Semaphore(int init)
{
	myPcbSem = new PcbSem(init);
}
Semaphore::~Semaphore() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
        delete myPcbSem;
        myPcbSem = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

}
int Semaphore::wait(Time maxTimeToWait) {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	int return_value = myPcbSem->wait(maxTimeToWait);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return return_value;
}
int Semaphore::signal(int n) {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	int return_value = myPcbSem->signal(n);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return return_value;
}
int Semaphore::val() const {
    return myPcbSem->val;
}

void Semaphore::turnOnPriorities(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	 myPcbSem->set_priority(1);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

