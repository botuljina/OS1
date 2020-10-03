//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    user.cpp
//   Date:    Jun 2019
//===========================================================================//
#include <iostream.h>
#include <stdlib.h>
#include <assert.h>
#include "Timer.h"
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

         // 1 <= N <= 19
Semaphore s(0);
//---------------------------------------------------------------------------//

volatile int theEnd=0;



//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Korisnicki program mora obavezno da definise ovu f-ju
//---------------------------------------------------------------------------//
void tick(){
}

//---------------------------------------------------------------------------//

class TestThread: public Thread{
public:
	TestThread():Thread() {pravo ='n';}
	~TestThread()
	{
		waitToComplete();
	}
	char pravo;
	void set_pravo()
	{
		 int r = rand() % 5;
		 if(r == 1 || r==2)
			 pravo='w';
		 else
			 pravo='r';
		 c = pravo;
		 syncPrintf("NIT-> %d IMA PRAVO %c \n",(int)getId(),pravo);
	}
protected:
	void run()
	{
		syncPrintf("metod run niti pocinje %d. pravo char = %c\n", (int)this->getId(),pravo);
			s.funkcija1(pravo);
			if(this->pravo == 'w')
			{
				s.entered_crit(1);
			}
			for(int i=0;i<32000;i++)
				{
					for(int j=0;j<16000;j++)
					{

					}
				}
			syncPrintf("metod run niti se zavrsava%d.\n", (int)this->getId());
			if(this->pravo == 'w')
			{
				s.entered_crit(0);
			}
			s.funkcija();


	}
};

int N = 10;
int userMain (int argc, char* argv[])
{
	intLock
	cout<<"userMain START!"<<endl;
	intUnlock
	
	TestThread t1[10];
	for(int i =0;i<N;i++){
		t1[i].set_pravo();
		t1[i].start();
		s.inc_started_threads();
	}


	intLock
	cout<<"userMain finished!"<<endl;
	intUnlock
	return 0;
}
//---------------------------------------------------------------------------//
