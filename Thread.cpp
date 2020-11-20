/*
 * Thread.cpp
 *
 *  Created on: Aug 18, 2020
 *      Author: OS1
 */

#include "Thread.h"
#include "Global.h"
#include "PCB.h"


void dispatch(){
	asm cli;

	Global::requestedSwitch = 1;
	Global::timer();

	asm sti;
};


Thread::Thread(StackSize stackSize, Time timeSlice){
		myPCB = new PCB(stackSize,timeSlice,this);

	}

Thread::~Thread() {
}

void Thread::start(){
		myPCB->start();
	}

void Thread::waitToComplete(){
		myPCB->waitToComplete();
	}

ID Thread::getRunningId(){return Global::running->id;}

ID Thread::getId(){return myPCB->getID();}

void Thread::run(){};

Thread* Thread::getThreadById(ID id){
	PCBNode* curr;
	for(curr = Global::pcbHead; curr!=0 ; curr=curr->next){
		if(curr->elem->id == id)
			return curr->elem->myThread;
	}
	return 0;
}

#ifdef KERNEL_SIGNAL

	 void Thread::signal(SignalId signal){
		 lock;
		 myPCB->signal(signal);
		 unlock;
	 }

	void Thread::registerHandler(SignalId signal, SignalHandler handler){
		lock;
		myPCB->registerHandler(signal,handler);
		unlock;
	}

	void Thread::unregisterAllHandlers(SignalId id){
		lock;
		myPCB->unregisterAllHandlers(id);
		unlock;
	}

	void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
		lock;
		myPCB->swap(id,hand1,hand2);
		unlock;
	}

	void Thread::blockSignal(SignalId signal){
		lock;
		myPCB->blockSignal(signal);
		unlock;
	}

	void Thread::blockSignalGlobally(SignalId signal){
		lock;
		PCB::blockSignalGlobally(signal);
		unlock;
	}

	void Thread::unblockSignal(SignalId signal){
		lock;
		myPCB->unblockSignal(signal);
		unlock;
	}

	void Thread::unblockSignalGlobally(SignalId signal){
		lock;
		PCB::unblockSignalGlobally(signal);
		unlock;
	}

#endif
