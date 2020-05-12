#include <dos.h>
#include "Pcb.h"
#include "Timer.h"

#include <iostream.h>
void PCB::setDefaultValuesForAllAttributes()
{
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
		myThread = t;

		myPcbState = initialized;

		stackSize = s;

		PcbTimeSlice = timeSlice;

		setDefaultValuesForAllAttributes();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}


void PCB::createStack()
{
/*
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
	stack[stackSize-6] = FP_SEG((Timer::wrapper)); //segmentstack
	stack[stackSize-7] = FP_OFF((Timer::wrapper)); //offset
#endif

#ifndef BCC_BLOCK_IGNORE
	//ss,sp,bp
	ss=FP_SEG(stack+stackSize-16);
	sp=FP_OFF(stack+stackSize-16);
#endif
	//po pravilu na pocetku je bp kod sp-a.
	bp = sp;
	//-8 do -15 su registri ax, bx, cx, dx, es, ds, si, di
	//na - 16 je bp
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
*/
	lock();
	//ss:sp, cs:ip, privremeni ss:sp
	static volatile unsigned tsp, tss, tip, tcs, oldss, oldsp;
	static unsigned *tempStack;
	//pravljenje steka za nit
	tempStack = new unsigned[this->stackSize];
	this->stack = tempStack;
	//dobijanje segmenta i ofseta novog steka
	tsp = this->sp = FP_OFF(tempStack+stackSize);
	tss = this->ss = FP_SEG(tempStack+stackSize);
	//dobijanje segmenta i ofseta wrapper f-je (to ce nam biti adresa povratka
	//kada se pozove sistemska funkcija (interrupt rutina) za promenu konteksta)
	//zato je potrebno simulirati poziv te interrupt f-je
	tip = FP_OFF(&(Timer::wrapper));
	tcs = FP_SEG(&(Timer::wrapper));
	//prelazak na stek nove niti sa steka funkcije
	asm {
		mov oldss, ss
		mov oldsp, sp
		mov ss, tss
		mov sp, tsp

		//stavljanje flegova na stek i eksplicitno postavljanje I bita
		//da bi bili dozvoljeni prekidi kada se vratimo u wrapper f-ju
		pushf

		pop ax
		or ax, 0000001000000000b	// bit 9 (I)
		push ax

		//stavljanje adrese povratka u wrapper f-ju cs:ip
		mov ax, tcs
		push ax
		mov ax, tip
		push ax
		//stavljanje registara ax,bx,cx,dx,es,ds,si,di,bp koji se pri
		//pozivu interrupt f-je automatski stavljaju na stek
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
		//sada je u potpunosti simuliran poziv interrupt f-je i kada se sledeci
		//put promeni kontekst pocece se izvrsavanje iz sistemske wrapper f-je
		//koja obmotava virtuelnu run() f-ju

		//vracamo stek pointer za nit jer se posle push-eva promenio
		mov tsp, sp
		//vracamo se na stek f-je
		mov sp, oldsp
		//stek segment je ostao isti
		mov ss, oldss
	}
	//nit dobija novu poziciju sp odakle ce se pri promeni konteksta nastaviti
	this->sp = tsp;
	unlock();
}

PCB::~PCB()
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	delete PcbQueueForWaitingThreads;
	delete stack;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
