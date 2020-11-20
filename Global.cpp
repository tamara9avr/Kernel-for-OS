/*
 * Global.cpp
 *
 *  Created on: Aug 18, 2020
 *      Author: OS1
 */

#include "Global.h"
#include <iostream.h>
#include <dos.h>
#include "PCB.h"
#include "Thread.h"
#include "SCHEDULE.H"


volatile int Global::lockFlag=1;

volatile PCB* Global::running=0;

PCB Global::mainPCB(defaultStackSize, defaultTimeSlice, 0);

Global::EmptyThread Global::emptyThread;

volatile Time Global::counter = mainPCB.timeSlice;

volatile int Global::requestedSwitch=0;

volatile unsigned Global::tsp=0,Global::tss=0,Global::tbp=0;

pInterrupt Global::oldTimer;

PCBNode *Global::pcbHead=0;

SemNode* Global::semHead = 0;

IVTEntry* Global::ivtEntries[256]= {0};

void tick();

Global::EmptyThread::EmptyThread():Thread(defaultStackSize,2){};
Global::EmptyThread::~EmptyThread(){}
void Global::EmptyThread::run(){
	while(1);
};

void unblockTimeThreads(KernelSem *semaphore){

	PCBNode* curr = semaphore->headBlocked;
	while (curr!=0){

		if(curr->elem->timeToWait !=0){

			if(--(curr->elem->timeToWait) == 0){

				curr->elem->status==PCB::READY;
				Scheduler::put(curr->elem);
				curr->elem->flagUnblocked = 1;
				PCBNode* hlp = curr;
				curr = curr->next;
				semaphore->headBlocked = Lists::deletePCBListElem(hlp->elem,semaphore->headBlocked);
				semaphore->val++;
			}

			else{
				curr = curr->next;
			}
		}

		else{

			curr = curr->next;
		}

	}

}

void interrupt Global::timer(...){


	if(!requestedSwitch){

		tick();

		if(Global::running->flagNoTimer==0)
			Global::counter--;

		SemNode *help = Global::semHead;

		while(help!=0){
			unblockTimeThreads(help->elem);

			help=help->next;
		}

	}
		if ((Global::counter == 0 && Global::running->flagNoTimer==0) || Global::requestedSwitch) {
			if(Global::lockFlag){

				Global::requestedSwitch=0;

#ifndef BCC_BLOCK_IGNORE					//asm{

				tsp = _SP;					// 		mov tsp, sp
				tss = _SS;					//		mov tss, ss
				tbp = _BP;					//		mov tbp, bp
											//}
#endif

				Global::running->sp = tsp;
				Global::running->ss = tss;
				Global::running->bp = tbp;

				if(Global::running->status!=PCB::FINISHED && Global::running->status!=PCB::BLOCKED ){
					Global::running->status = PCB::READY;
					Scheduler::put((PCB*)Global::running);
				}


					Global::running= Scheduler::get();
					if(Global::running == 0){
						Global::running = Global::emptyThread.myPCB;
					}

					tsp = Global::running->sp;
					tss = Global::running->ss;
					tbp = Global::running->bp;


	#ifndef BCC_BLOCK_IGNORE

					_SP = tsp;					//Writing directly in stack pointer register
					_SS = tss;					//					  stack segment
					_BP = tbp;					//					  base pointer

	#endif



					Global::running->status = PCB::ACTIVE;
					Global::counter = Global::running->timeSlice;

#ifdef KERNEL_SIGNAL

					((PCB*)Global::running)->executeSignals();
					if(Global::running->status == PCB::FINISHED){
						delete Global::running->stack;
					}

#endif

		}
			else Global::requestedSwitch = 1;

	}

	if(!Global::requestedSwitch) {

		Global::oldTimer();
	}

};


#ifdef KERNEL_SIGNAL

void signal0(){
	lock;
	while(Global::running->head!=0){
		Global::running->head->elem->status=PCB::READY;
							Scheduler::put(Global::running->head->elem);
							Global::running->head = Global::running->head->next;
						}


	Global::running->status == PCB::FINISHED;
	Global::running->waiting = Lists::deleteSignalList(Global::running->waiting);
	Global::running->waiting = 0;



				unlock;
				dispatch();
}

#endif

void Global::init(){
	asm cli;

#ifndef BCC_BLOCK_IGNORE

	Global::oldTimer = getvect(0x8);
	setvect(0x8, timer);

	tss = _SS;
	tsp = _SP;
	tbp = _BP;

#endif

	mainPCB.ss = tss;
	mainPCB.sp = tsp;
	mainPCB.bp = tbp;;

	mainPCB.myThread = &emptyThread;
	mainPCB.status=PCB::ACTIVE;
	running = &mainPCB;

#ifdef KERNEL_SIGNAL
	for(int i = 0; i<16; i++){
		PCB::globallyBlocked[i]=0;
	}

	((PCB*)Global::running)->registerHandler(0,signal0);

#endif

	asm sti;
}


void Global::restore(){
	asm cli;

#ifndef BCC_BLOCK_IGNORE

	setvect(0x8, Global::oldTimer);


#endif

	Lists::deleteSemList(Global::semHead);
	Lists::deletePCBList(Global::pcbHead);

	delete semHead;
	delete pcbHead;


	asm sti;
}

