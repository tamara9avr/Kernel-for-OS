/*
 * krlev.cpp
 *
 *  Created on: Aug 22, 2020
 *      Author: OS1
 */

#include "krlev.h"

#include "PCB.h"
#include "Global.h"
#include "SCHEDULE.H"

#include <iostream.h>

KernelEv::KernelEv(IVTNo ivtNo, Event *ev) {
	num = ivtNo;
	lock;
	myPCB = (PCB*)Global::running;
	myImpl = ev;

	Global::ivtEntries[ivtNo]->myEvent = this;
	unlock;
}

KernelEv::~KernelEv() {
	Global::ivtEntries[num]->myEvent = 0;
}

void KernelEv::wait(){
	lock;
	if(Global::running==myPCB){
			Global::running->status = PCB::BLOCKED;
			unlock;
			dispatch();
		}
	unlock;
}

void KernelEv::signal(){
	lock;
		myPCB->status = PCB::READY;
		Scheduler::put((PCB*)myPCB);
	unlock;
}

