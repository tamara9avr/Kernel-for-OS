/*
 * Event.h
 *
 *  Created on: Aug 22, 2020
 *      Author: OS1
 */

#ifndef OS_EVENT_H_
#define OS_EVENT_H_

#include "IVTEntry.h"
#include "Global.h"

typedef unsigned char IVTNo;


#define PREPAREENTRY(numEntry, oldFlag)\
void interrupt inter ## numEntry(...); \
IVTEntry newEntry ## numEntry(numEntry, inter ## numEntry); \
void interrupt inter ## numEntry(...) {\
		newEntry ## numEntry.signal();\
		if (oldFlag == 1)\
			newEntry ## numEntry.callOld();\
};

class KernelEv;

class Event {

public:

	 Event (IVTNo ivtNo);

	 ~Event ();

	 void wait ();

protected:

	 friend class KernelEv;

	 void signal(); // can call KernelEv

private:

	 KernelEv* myImpl;

};


#endif /* OS_EVENT_H_ */
