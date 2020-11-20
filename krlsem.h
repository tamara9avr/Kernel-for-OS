/*
 * krlsem.h
 *
 *  Created on: Aug 20, 2020
 *      Author: OS1
 */

#ifndef _KRLSEM_H_
#define _KRLSEM_H_

#include "semaphor.h"
#include "PCB.h"

struct PCBNode;

class KernelSem {
public:
	KernelSem(int init, Semaphore* sem);
	virtual ~KernelSem();

	int wait (Time maxTimeToWait);

	int signal(int n = 0);

	int value() const;

protected:

	void unblock();

	friend class Global;

	 friend class BoundedBuffer;

	friend void unblockTimeThreads(KernelSem*);

private:

	int val;

	Semaphore* mySem;

	PCBNode* headBlocked;					//PCBs that are regularly blocked;

	int numOfBlocked;

	volatile int flagUnblocked;

};

#endif /* OS_KRLSEM_H_ */
