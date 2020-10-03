#include <dos.h>
#include "Pcb.h"
#include "Timer.h"

#include <iostream.h>
void PCB::setDefaultValuesForAllAttributes()
{
	SemRetVal=0;
	stack=0;
	ss=0;
	sp=0;
	bp=0;
    PcbQueueForWaitingThreads= new queue();
	TimePassedForThreadPerforming=0;
}

PCB::PCB(Thread* t,StackSize s,volatile unsigned long timeSlice)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
		//cout<<"Uso u PCB konstruktor\n";
		this->friends_queue = new queue();
		myThread = t;

		myPcbState = initialized;

		stackSize = s;

		PcbTimeSlice = timeSlice;

		Timer::globalthreadID++;
	    threadID = Timer::globalthreadID;

		setDefaultValuesForAllAttributes();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}


void PCB::createStack()
{

#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	stackSize /=sizeof(unsigned);
	stack= new unsigned[stackSize];

#ifndef BCC_BLOCK_IGNORE
	//nakon pravljenja steka, potrebno ga je inicijalizovati po odredjenim pravilima
	stack[stackSize-1]=FP_SEG(myThread);
	stack[stackSize-2]=FP_OFF(myThread);
#endif


	//callbak wrapper funkcije, koja nikad nece doci do kraja, poslednja linija wrapper-a je ustvari dispatch
	//stack[stackSize-3]=FP_SEG(myThread);
	//stack[stackSize-4]=FP_SEG(myThread);

	//PSW
	stack[stackSize-5] = 0x200;

#ifndef BCC_BLOCK_IGNORE
	//PC[CS:IP]
	stack[stackSize-6] = FP_SEG((Timer::wrapper)); //segment
	stack[stackSize-7] = FP_OFF((Timer::wrapper)); //offset
#endif

#ifndef BCC_BLOCK_IGNORE
	//ss,sp,bp
	ss=FP_SEG(stack+stackSize-16);
	sp=FP_OFF(stack+stackSize-16);
#endif
	//po pravilu na pocetku je bp kod sp-a.
	bp = sp;
	//-8 do -15 su registri ax pa bx, cx, dx, es, ds, si, di
	//na - 16 je bp
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
/*
 *  sa ovim delom mi je radilo, obrati paznju na ovo
 *  IMPLEMENTIRAN JE DOSTA SMEKERSKIJI NACIN, ZA SLUCAJ DA NE RADI, VRATI OVO!!!!!!!
 *
 *
	lock();
	//ss:sp, cs:ip, privremeni ss:sp
	static volatile unsigned tsp, tss, tip, tcs, oldss, oldsp;
	static unsigned *tempStack;
	//pravljenje steka
	tempStack = new unsigned[this->stackSize];
	this->stack = tempStack;
	//segment i ofset novog steka
	tsp = this->sp = FP_OFF(tempStack+stackSize);
	tss = this->ss = FP_SEG(tempStack+stackSize);


	tip = FP_OFF(&(Timer::wrapper));
	tcs = FP_SEG(&(Timer::wrapper));
	//prelazak na stek nove niti sa steka funkcije
	asm {
		mov oldss, ss
		mov oldsp, sp
		mov ss, tss
		mov sp, tsp


		pushf

		pop ax
		or ax, 0000001000000000b	// bit 9 (I)
		push ax


		mov ax, tcs
		push ax
		mov ax, tip
		push ax

		mov ax, 0
		push ax
		push bx
		push cx
		push dx
		push es
		push ds
		push si
		push di
		push bp



		mov tsp, sp

		mov sp, oldsp

		mov ss, oldss
	}

	this->sp = tsp;
	unlock();
*/
}

PCB::~PCB()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	delete PcbQueueForWaitingThreads;
	delete stack;
	delete friends_queue;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
