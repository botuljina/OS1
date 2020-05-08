#include "Timer.h"

void  deleteStaticElements()
{
	//ne mogu da uticem kad ce se static atributi tajmer klase obrisati ali za svaki slucaj oslobadjam se pokazivaca u ovoj metodi
	//delete Timer::globalQueueForGettingIds;
}
extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    Timer::start_system_initialization();

    int value = userMain(argc, argv);

    Timer::restore_system();

    deleteStaticElements();
}
