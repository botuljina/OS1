// File: thread.h
#ifndef _thread_h_
#define _thread_h_
#include "Pcb.h"

typedef unsigned long StackSize;

const StackSize defaultStackSize = 4096;

typedef unsigned int Time; // time

typedef unsigned long int ID;

const Time defaultTimeSlice = 2; // default = 2*55ms


class Thread {

public:

  Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
  //void assignIDtoThread();

  void start();

  void waitToComplete();
  int checkIfNeedsWaiting();

  virtual ~Thread();

  ID getId();

  static ID getRunningId();
  static Thread* getThreadById(ID id);

  Thread* mypair;
  int in_crit;
protected:
  friend Timer;
  friend queue;
  virtual void run() {}

  PCB* threadPCB;

private:



};


void wrapper();
void dispatch();
extern void tick();
Time minTimeSlice();

#endif
