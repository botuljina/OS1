#ifndef _semaphor_h_
#define _semaphor_h_

#include "PcbSem.h"


typedef unsigned int Time;


class Semaphore {
public:
    Semaphore(int init=1);
    virtual ~Semaphore();
    virtual int wait(Time maxTimeToWait);
    virtual int signal(int n=0);

    int val() const;
    void turnOnPriorities();
private:
    PcbSem* myPcbSem;
};
#endif
