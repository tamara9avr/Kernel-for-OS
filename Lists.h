/*
 * Lists.h
 *
 *  Created on: Sep 7, 2020
 *      Author: OS1
 */

#ifndef LISTS_H_
#define LISTS_H_

#include "Thread.h"

class PCB;
class KernelSem;

struct PCBNode{						//Nodes for linked list
		PCB* elem;
		PCBNode* next;
	};


struct SemNode{
	KernelSem* elem;
	SemNode* next;
};

#ifdef KERNEL_SIGNAL

struct HandlerNode{
	SignalHandler handler;
	HandlerNode* next;
};

struct Signal{
	HandlerNode* head;
	int blocked;
};

struct SignalNode{
	SignalId signal;
	SignalNode* next;
};

#endif

class Lists {
public:


	static PCBNode* addInPCBList(PCB*, PCBNode*);

	 static PCBNode* deletePCBList(PCBNode*);

	 static PCBNode* deletePCBListElem(PCB*, PCBNode*);

	 static SemNode* addInSemList(KernelSem*, SemNode*);

	 static SemNode* deleteSemList(SemNode*);

	 static SemNode* deleteSemListElem(KernelSem*, SemNode*);

#ifdef KERNEL_SIGNAL

	 static HandlerNode* addHandler(SignalHandler, HandlerNode*);

	 static HandlerNode* deleteHandlerList(HandlerNode*);

	 static HandlerNode* deleteHandlerElem(SignalHandler, HandlerNode*);

	 static HandlerNode* swapHandlers(SignalHandler, SignalHandler, HandlerNode*);

	 static SignalNode* addInSignalList(SignalId, SignalNode*);

	 static SignalNode* deleteSignalList(SignalNode*);

	 static SignalNode* deleteSignalListElem(SignalId, SignalNode*);

#endif
};

#endif
