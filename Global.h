/*
 * Global.h
 *
 *  Created on: Aug 18, 2020
 *      Author: OS1
 */

#ifndef OS_GLOBAL_H_
#define OS_GLOBAL_H_

#include "PCB.h"
#include "krlsem.h"
#include "IVTEntry.h"
#include "Lists.h"


class Lists;
class PCB;
class IVTEntry;

typedef void interrupt (*pInterrupt)(...);

#define lock Global::lockFlag=0;

#define unlock Global::lockFlag=1;\
	if(Global::requestedSwitch)\
	dispatch();



class Global{

public:

	class EmptyThread : Thread{				//EmptyThread is used when Scheduler returns nullptr. timeSlice is 1, and run contains only empty loop.
	public:
		EmptyThread();
		~EmptyThread();
	protected:
		void run();
		friend class Global;
	};


	 static EmptyThread emptyThread;

	 static PCBNode *pcbHead;

	 static SemNode *semHead;

	 static IVTEntry* ivtEntries[256];

	 static volatile int lockFlag;

	static volatile Time counter;

	static volatile PCB* running;

	static PCB mainPCB;

	static volatile int requestedSwitch;

	static volatile unsigned tsp,tss,tbp;

	static void init();

	static void restore();

	static pInterrupt oldTimer;

	static void interrupt timer(...);

};



#endif /* OS_GLOBAL_H_ */
