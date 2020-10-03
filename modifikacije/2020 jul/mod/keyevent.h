//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    keyevent.h
//   Date:    Jun 2019
//===========================================================================//

#ifndef _OS1_KEYBOARD_EVENT_
#define _OS1_KEYBOARD_EVENT_

#include <thread.h>

class BoundedBuffer;



class KeyboardEvent :public Thread{
public:

	KeyboardEvent (int i);
	virtual ~KeyboardEvent () {waitToComplete();}
	Thread* clone() const { return new KeyboardEvent(0); };
	friend Event;
protected:

	virtual void run();

	char znak;

private:
  
};




#endif  // _OS1_KEYBOARD_EVENT_
