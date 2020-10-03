//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    user.cpp
//   Date:    Jun 2019
//===========================================================================//
#include <iostream.h>
#include <stdlib.h>
#include <assert.h>

#include "bounded.h"
#include "intLock.h"
#include "keyevent.h"
#include "user.h"
#include "Event.h"
#include "semaphor.h"
#include "sPrintf.h"
//---------------------------------------------------------------------------//
//  Otkomentarisati ukoliko se testiraju signali
//---------------------------------------------------------------------------//
// #define SIGNAL
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//  Ovo se menja u testu
//---------------------------------------------------------------------------//

Time TIME_SLICE = 2;   // 0 ili defaultTimeSlice

int N = 3;            // 1 <= N <= 19

//---------------------------------------------------------------------------//

volatile int theEnd=0;

class Producer : public Thread {
public:
	Producer (BoundedBuffer* bb, char y, Time time_slice);
	virtual ~Producer() {waitToComplete(); }
    Thread* clone() const { return new Producer(myBuffer, x, time_slice_clone); }
protected:
	virtual void run ();
	char produce() {return x;}; // Produce an item

private:
	Time time_slice_clone;
	BoundedBuffer* myBuffer;
	char x;
	Semaphore sleep;
};



//---------------------------------------------------------------------------//
class Consumer : public Thread {
public:
	Consumer (BoundedBuffer* bb) : Thread(defaultStackSize, 0), myBuffer(bb), sleep(0) {}
	virtual ~Consumer() {waitToComplete(); }
	Thread* clone() const { return new Consumer(myBuffer); }
protected:
	virtual void run ();
	void consume(char p); // Consume an item

private:
	BoundedBuffer* myBuffer;
	Semaphore sleep;
};




//---------------------------------------------------------------------------//
// Korisnicki program mora obavezno da definise ovu f-ju
//---------------------------------------------------------------------------//
void tick(){
}

//---------------------------------------------------------------------------//


Producer::Producer (BoundedBuffer* bb, char y, Time time_slice) 
: Thread(defaultStackSize, time_slice),myBuffer(bb), x(y), sleep(0), time_slice_clone(time_slice) {}
	
	void Producer::run () {
	while(!theEnd) {
		char d = produce();
		myBuffer->append(d);
		assert(1 != sleep.wait(10));
	}
}

//---------------------------------------------------------------------------//


void Consumer::consume(char p) {
	intLock
	cout<<p<<" ";
	intUnlock
} // Consume an item

void Consumer::run () {
		
	int i = 0;
	while(!theEnd) {
		char d = myBuffer->take();
		consume(d);
		if (i++ == 40) {
			assert(1 != sleep.wait(5));
			i = 0;
		}else for(int j=0;j<200;j++);
	}
 
	intLock
	cout<<endl<<"ESC pressed - empty the buffer!"<<endl;
	intUnlock
	
	while (myBuffer->fullCount()){
		char d = myBuffer->take();
		consume(d);
		dispatch();
	}
	
	
	intLock
	cout<<endl<<"Happy End"<<endl;
	intUnlock
}

#ifdef SIGNAL
Semaphore signalTest(0);
void signal1() {
	signalTest.signal();
}

#endif
Semaphore s(0);

class Test: public Thread
{
public:
	Test():Thread(4086,1) {}
	~Test() {waitToComplete();}
	 void assign_prior();
protected:
	void run();
};
void Test::assign_prior()
{
	int r = rand()%3;
	switch(r)
	{
	case 0: this->myPrior =low; break;
	case 1: this->myPrior =medium; break;
	case 2: this->myPrior =high; break;
	default: this->myPrior =low; break;
	}
	//syncPrintf("%d,%d\n",(int)getId(),myPrior);
}
void Test::run()
{
	while(1)
	{
		assign_prior();
		s.wait(0);
	}
}

class SignalThread: public Thread
{
public:
	SignalThread():Thread(4096,1) {}
	~SignalThread(){waitToComplete();}
protected:
	void run();
};
void SignalThread::run(){
	while(1)
	{
		s.wait(80);

		int r = rand()%5+1;
		syncPrintf("******************\n");
		syncPrintf("%d\n",r);
		syncPrintf("******************\n");
		s.signal(r);
	}
}
int userMain (int argc, char* argv[])
{
	intLock
	cout<<"userMain starts!"<<endl;
	intUnlock
	int l = 10;
	int i =0;
	Test* t[10];
	SignalThread* sig = new SignalThread();

	for(i=0;i<l;i++)
	{
		t[i] = new Test();
		t[i]->start();
	}

	sig->start();

	for(i=0;i<l;i++)
	{
		delete t[i];
	}
	
	delete sig;
	intLock
	cout<<"userMain finished!"<<endl;
	intUnlock
	return 0;
}
//---------------------------------------------------------------------------//
