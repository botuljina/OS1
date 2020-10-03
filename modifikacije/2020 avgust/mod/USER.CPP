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
#include "Timer.h"
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

Semaphore sem(10);
class Testhread:public Thread
{
private:
	Event *evPok;
	Semaphore* semPok;
public:
	int theEnd;
	Testhread();
	virtual ~Testhread () {
		waitToComplete();
		delete evPok;
		delete semPok;
	}

protected:
	void run();


};
void Testhread::Testhread():Thread(4096,1){

	semPok = &sem;
	theEnd = 0;
}

void Testhread::run()
{
	char scancode, status, znak;
		//syncPrintf("KRENUO RUN");
		Event event9(9);
		evPok = &event9;
		while (!theEnd) {
			evPok->wait();
			do{
						status = inportb(0x64); // ocitava statusni reg. sa 64h


						if (status & 0x01){           // Can I read?
							scancode = inportb(0x60);

							if (scancode==-127){
								theEnd = 1;

							}else {
								if (scancode&0x80) {

								}
							}

						};
			       //////////////////////
						asm{
							cli
							in      al, 61h         //; Send acknowledgment without
							or      al, 10000000b   //;   modifying the other bits.
							out     61h, al         //;
							and     al, 01111111b   //;
							out     61h, al         //;
							mov     al, 20h         //; Send End-of-Interrupt signal
							out     20h, al         //;
							sti
						}

					 }while (!theEnd && status & 0x01); //dok se ocitava takav status da je pritisnut neki taster
				    //syncPrintf("USO\n");
					semPok->wait(20);
				    //syncPrintf("PRSOSO\n");
				}// while
}

int userMain (int argc, char* argv[])
{
	int i =0;
	Testhread *t[50];
	for(i=0;i<50;i++)
	{
		t[i] = new Testhread();
		t[i] ->start();
		//Timer::globalQueueForGettingIds->printQueueElements();
	}


	for(i=0;i<50;i++)
		{
			delete t[i];
		}
}
//---------------------------------------------------------------------------//
