#include "Timer.h"
#include "thread.h"
#include <iostream.h>
#include <stdio.h>

extern int syncPrintf (const char* format, ... );

const int n = 50;

void tick() {}

class TestThread : public Thread
{
public:

	TestThread(int id): Thread(4096, 1), myID(id) {};
	~TestThread()
	{
		waitToComplete();
	}
protected:
	int myID;
	void run();

};

class WaitThread: public Thread
{
public:
	WaitThread(TestThread *t1, TestThread *t2): Thread(4096, 1)
	{
		t1_ = t1;
		t2_ = t2;
	};

	~WaitThread()
		{
			waitToComplete();
		}

protected:
	void run()
	{
		syncPrintf("Starting tests...\n");
		t1_->waitToComplete();
		syncPrintf("Test 1 completed!\n");
		t2_->waitToComplete();
		syncPrintf("Test 2 completed!\n");
	}
private:
	TestThread *t1_,*t2_;
};

void TestThread::run()
{

	syncPrintf("Thread %d adresa: %p: loop1 starts\n", getId(), this);

	for(long i=0;i<8000;i++) {

		for (long j = 0; j < 32000; j++);
	}
	syncPrintf("FOR VRTESKA\n");
	//Thread::sleep(myID*20);

	syncPrintf("Thread %d: loop1 ends, dispatch\n",getId());

	dispatch();

	syncPrintf("Thread %d: loop2 starts\n", getId());

	for (int k = 0; k < 8000; k++) { for (long j = 0; j < 32000; j++); }
	//Thread::sleep(myID*20);

	syncPrintf("Thread %d: loop2 ends\n", getId());


}
int userMain(int argc, char** argv)
{
	syncPrintf("User main starts\n");
	//TestThread *t = new TestThread(1);
	//t->start();

	TestThread *t[n];
	for(int i = 0; i < n; i++) {
		t[i] = new TestThread(i);
		syncPrintf("Thread %d adresa: %ld:\n", t[i]->getId(), t[i]);
		syncPrintf("Thread: %d made\n",i);
	}
	for(int j = 0; j < n; j++) {
		t[j]->start();
		//syncPrintf("Thread: %d started\n",j);
	}


	for (long k=0; k<5; k++) {
		for(long i=0;i<8000;i++) {
			for (long j = 0; j < 32000; j++);

		}
		//Thread::sleep(10);

		syncPrintf("Usermain %d running!\n", Thread::getRunningId());
		//Thread *m = Thread::getThreadById(3);
		//syncPrintf("%p\n", m);
		Timer::dispatch();
	}//

	for(int l = 0; l < n; l++) {
		delete t[l];
	}
	syncPrintf("User main ends\n");
	return 0;
}
