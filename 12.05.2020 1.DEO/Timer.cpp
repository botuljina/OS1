/*
 * Timer.cpp
 *
 *  Created on: Apr 17, 2020
 *      Author: OS1
 */
#include "Timer.h"
#include <iostream.h>
#include <dos.h>
#include "queue.h"
#include "Thread.h"
#include "Pcb.h"
#include "Idle.h"
#include "SCHEDULE.H"
#include "sPrintf.h"

class queue;
class PCB;
class Idle;
class Thread;

unsigned oldTimerOFF = 0;
unsigned oldTimerSEG = 0;

queue* Timer::globalQueueForGettingIds=0;
Thread* Timer::startingThread= 0;
Idle* Timer::idleThread = 0;
volatile PCB* Timer::running=0;
volatile unsigned Timer::dispatched=0;
unsigned long Timer::globalthreadID=0;
void interrupt (*Timer::oldRoutine)(...) = 0;
int Timer::testingPhase =0;
//---------------------------------------------------------
void Timer::SetAllPrefiniedAtrributes()
{

	Timer::startingThread= new Thread(4096, minTimeSlice());
	Timer::startingThread->threadPCB->myPcbState = ready;
	Timer::running= (volatile PCB*)Timer::startingThread->threadPCB;
	Timer::idleThread = new Idle();

}
//Funkcija: Podmece metod timerRoutine da bude na ulazu 0x08 u IV tabeli.
void Timer::start_system_initialization()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
 //da napomenem, nisam na pocetku stavio da je ovaj pokazivac null
#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(0x08);
	setvect(0x08,timerRoutine);
	SetAllPrefiniedAtrributes();
	Timer::idleThread->start();
#endif

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

//---------------------------------------------------------

//Funkcija: Eksplicitni poziv promene konteksta
void Timer::dispatch()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	//cout<<"Krenuo nevaljali dispatch"<<endl;
	dispatched = 1;
	timerRoutine();
	dispatched = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}


//pomocne promenljive za prekid tajmera, kao sto pise u K3N
unsigned tsp = 0;
unsigned tss = 0;
static volatile PCB *newThread;
//Funkcija: Vrsi promenu konteksta.
void interrupt Timer::timerRoutine(...)
{
	//syncPrintf("NALAZI SE U TIMER ROUTINI");
//timerRoutine ima 3 toka kontrola
//1) ako nije eksplictni poziv
	if(!dispatched)
	{
		tick();
		(*oldRoutine)();
	}

	if (!dispatched && running->PcbTimeSlice != 0)
			running->TimePassedForThreadPerforming += (volatile unsigned long)minTimeSlice();

	if (!dispatched && (running->TimePassedForThreadPerforming < running->PcbTimeSlice || running->PcbTimeSlice == 0) ) return;

	dispatched = 0;

	if(running->myPcbState == ready && (PCB*)running != idleThread->getIdlePCB())
			Scheduler::put((PCB*)running);

	newThread = Scheduler::get();
	if (newThread == 0) newThread = idleThread->threadPCB; //ako je prazan Scheduler koristimo idle nit

	//if (newThread->myPcbState != ready) continue; //ako je dohvatio nit koja nije spremna ponavljamo dohvatanje
	//promena konteksta se svodi na promenu steka niti

#ifndef BCC_BLOCK_IGNORE
	asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
		}
#endif
	 running->sp = tsp;
	 running->ss = tss;
	 running = newThread;
	 tsp = running->sp;
	 tss = running->ss;
#ifndef BCC_BLOCK_IGNORE
	 		asm {
	 			mov sp, tsp
	 			mov ss, tss
	 		}
#endif
}

void Timer::restore_system() {
	//da li su sve niti ugasene sem starter niti
	if( (PCB*)running != startingThread->threadPCB ) return;
#ifndef BCC_BLOCK_IGNORE
	lock();
	setvect(0x08, oldRoutine);
#endif
	delete globalQueueForGettingIds;
	delete startingThread;
	delete idleThread;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

//------------------------------------------------------------------------------------------
//funkcija omotac virtuelne f-je run()
void Timer::wrapper() {
	//ulaskom u wrapper, stek niti koju pokrece wrapper je prazan
	//pokrece izvrsavanje tela niti
	//cout<<"Uspeo ulaz u wraper funkciju\n";

	running->myThread->run();
	//nit se zavrsila
	//pocinje kriticna sekcija koja se ne sme prekidati
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	//markira nit kao gotovu, da je promena konteksta ne bi vratila u Scheduler i da se roditelj ne bi blokirao na waitToComplete
	running->myPcbState = finished;
	//aktiviramo sve niti koje su bile blokirane na ovoj
	PCB* temp;
	while( running->PcbQueueForWaitingThreads->getQueueSize() > 0 )
	{
		if(temp!=0)
		{
			if(testingPhase)
				syncPrintf("zavrsio se run za nit sa id-jem %d i vadim iz njenog queue-a\n",running->myThread->threadID);
			temp = running->PcbQueueForWaitingThreads->get();

			temp->myPcbState = ready;
			Scheduler::put(temp);
		}

	}
	if(testingPhase)
		syncPrintf("zavrsio se run za nit sa id-jem %d i vadim iz njenog queue-a\n",running->myThread->threadID);
	//prelazak na drugu nit, jer na steku nema povratne adrese za povratak iz wrapper-a
	dispatch();
}
