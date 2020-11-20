/*
 * PCBc.cpp
 *
 *  Created on: Aug 17, 2020
 *      Author: OS1
 */

#include "PCB.h"
#include "SCHEDULE.H"
#include <dos.h>
#include <iostream.h>

ID PCB::cnt = 0;

void PCB::wrapper(){

		Global::running->myThread->run();

		lock;

		while(Global::running->head!=0){
			Global::running->head->elem->status=READY;
			Scheduler::put((PCB*)Global::running->head->elem);
			Global::running->head = Global::running->head->next;
		}

#ifdef KERNEL_SIGNAL
		if(Global::running->parentThread!=0){
			((PCB*)Global::running)->parentThread->signal(1);
		}

		if(Global::running->signals[2].blocked==0 && PCB::globallyBlocked[2]==0){
			HandlerNode* curr = Global::running->signals[2].head;
			while(curr!=0){
				curr->handler();
				curr = curr->next;
			}
		}

#endif

		Global::running->status = FINISHED;

		unlock;
		dispatch();
}


PCB::PCB(StackSize stackSize, Time timeSlice, Thread* thread){

		activated = 0;
		head = 0;

		flagNoTimer = timeSlice==0?1:0;

		status = CREATED;
		id = cnt++;

		this->stackSize = stackSize;
		this->timeSlice=timeSlice;
		timeToWait = 0;
		myThread=thread;

		stack = new unsigned[stackSize/sizeof(unsigned long)];

		stack[stackSize-1] =0x200;

#ifndef BCC_BLOCK_IGNORE
		stack[stackSize -2] = FP_SEG(wrapper);
		stack[stackSize-3] = FP_OFF(wrapper);

		sp = FP_OFF(stack+stackSize-12);
		ss = FP_SEG(stack+stackSize-12);
#endif


		bp = sp;
		Global::pcbHead = Lists::addInPCBList(this,Global::pcbHead);

#ifdef KERNEL_SIGNAL
		parentThread = (PCB*)Global::running;
		waiting = 0;

		if(parentThread!=0){

			for(int i = 0; i<16; i++){
				HandlerNode* curr = parentThread->signals[i].head;
				while(curr!=0){
					signals[i].head = Lists::addHandler(curr->handler, signals[i].head);
					curr=curr->next;
				}
				this->signals[i].blocked = parentThread->signals[i].blocked;
			}
		}

		else{

			for(int i = 0; i<16; i++){
				this->signals[i].head = 0;
				this->signals[i].blocked = 0;
			}
		}
#endif


	}

void PCB::start(){						//Start method can only be called once.
	lock;
	if(activated==0){
			status = READY;
			Scheduler::put(this);
			activated=1;
		}
	unlock;
}


void PCB::waitToComplete(){
	lock;
	if(Global::running!=this){
		if(status!=FINISHED){
				Global::running->status = BLOCKED;

				head = Lists::addInPCBList((PCB*)Global::running, this->head);

				unlock;
				dispatch();

		}
	}
		unlock;
	}


ID PCB::getID() const {return id;}

PCB::~PCB() {

	delete [] stack;

}



#ifdef KERNEL_SIGNAL

unsigned PCB::globallyBlocked[16];

void PCB::signal(SignalId signal){

	if(signal<16)
	waiting = Lists::addInSignalList(signal, waiting);

}

void PCB::registerHandler(SignalId signal, SignalHandler handler){


	if(signal<16)
	signals[signal].head = Lists::addHandler(handler,signals[signal].head);
}

void PCB::unregisterAllHandlers(SignalId id){

	if(id<16)
	signals[id].head = Lists::deleteHandlerList(signals[id].head);
}

void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){

	if(id<16)
	signals[id].head = Lists::swapHandlers(hand1, hand2, signals[id].head);
}

void PCB::blockSignal(SignalId signal){

	if(signal<16)
	signals[signal].blocked = 1;

}

void PCB::blockSignalGlobally(SignalId signal){

	if(signal<16)
	globallyBlocked[signal] = 1;
}

void PCB::unblockSignal(SignalId signal){

	if(signal<16)
	signals[signal].blocked = 0;

}

void PCB::unblockSignalGlobally(SignalId signal){

	if(signal<16)
	globallyBlocked[signal] = 0;
}

void PCB::executeSignals(){

	SignalNode* curr = waiting;

	while(curr!=0){


		lock;

		SignalId id = curr->signal;


		if(signals[id].blocked==0 && globallyBlocked[id]==0){

/*			if(id==0){

				while(head!=0){
							head->elem->status=READY;
							Scheduler::put(head->elem);
							head = head->next;
						}


				status == FINISHED;
				waiting = Lists::deleteSignalList(waiting);
				waiting = 0;
				for(int i = 0; i<16; i++){
					this->unregisterAllHandlers(i);
				}


				unlock;
				dispatch();
			}
			else{
*/
				HandlerNode* hnd = signals[id].head;
				while(hnd!=0){
					hnd->handler();
					hnd = hnd->next;
				}


				curr = curr->next;
				waiting = Lists::deleteSignalListElem(id, waiting);
			}
//		}
		else{
			curr = curr->next;
		}
		unlock;
	}
}

#endif
