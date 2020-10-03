#include "Timer.h"


extern int userMain(int argc, char* argv[]);


int main(int argc, char* argv[])
{
    Timer::start_system_initialization();

    int value = userMain(argc, argv);

    Timer::restore_system();

}
