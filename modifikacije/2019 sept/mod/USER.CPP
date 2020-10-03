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


class args{
public:
	args(int arr[]){
		for(int i=0;i<2;i++)
		{
			n[i]=arr[i];
		}

	}
	int n[];
};
args* abc = 0;
class TestThread: public Thread
{
public:
	TestThread(void (*f) (void*), void* param,StackSize stackSize,Time timeSlice):Thread(stackSize,timeSlice)
	{
		syncPrintf("EVE GA OVDE");
		pok_na_funkciju = f;

	    b = (args*)param;
		for(int i =0; i<2; i++)
		{
			syncPrintf("** %d\n",b->n[i]);
		}
		this->start();
	}
	~TestThread()
		{
			waitToComplete();
		}
	void (*pok_na_funkciju)(void*);
	args* b;
	virtual void run()
	{
		this->pok_na_funkciju((void*)b);
	}
};

static void sum(void* _a)
{
	args* a = (args*) _a;
	int i=0;
	syncPrintf("CAO MALA USPEO RAN\n");

	for(i =0; i<2; i++)
	{
		a->n[i]+=1;
	}

	for(i =0; i<2; i++)
	{
		syncPrintf("%d\n",a->n[i]);
	}

}
void tick(){
}

int userMain (int argc, char* argv[])
{
	syncPrintf("TEST START");
	int n[2];
	n[0] =1;
	n[1] =2;

	abc = new args(n);

	TestThread *t = new TestThread(&sum,(void*)abc,4096,2);


	//delete abc;
	delete t;
	return 0;
}
//---------------------------------------------------------------------------//
