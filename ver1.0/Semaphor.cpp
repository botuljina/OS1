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
    return myPcbSem->wait(maxTimeToWait);
}
int Semaphore::signal(int n) {
    return myPcbSem->signal(n);
}
int Semaphore::val() const {
    return myPcbSem->val;
}
