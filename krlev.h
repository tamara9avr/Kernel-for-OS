/*
 * krlev.h
 *
 *  Created on: Aug 22, 2020
 *      Author: OS1
 */

#ifndef OS_KRLEV_H_
#define OS_KRLEV_H_

#include "event.h"

class PCB;

class KernelEv {
public:
	KernelEv (IVTNo ivtNo, Event *ev);

	 ~KernelEv ();

	 void wait ();

	 void signal();

protected:

private:

	 Event* myImpl;

	 IVTNo num;

	 PCB *myPCB;

};

#endif /* OS_KRLEV_H_ */
