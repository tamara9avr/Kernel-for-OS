/*
 * krlsem.cpp
 *
 *  Created on: Aug 20, 2020
 *      Author: OS1
 */

#include "krlsem.h"
#include "Global.h"
#include "semaphor.h"
#include "SCHEDULE.H"
#include <iostream.h>
#include "Lists.h"

KernelSem::KernelSem(int init, Semaphore* sem):val(init),mySem(sem) {

	headBlocked =0;
	numOfBlocked  = 0;

	Global::semHead = Lists::addInSemList(this, Global::semHead);
};

KernelSem::~KernelSem() {
	Global::semHead = Lists::deleteSemListElem(this, Global::semHead);
};


void KernelSem::unblock(){

		headBlocked->elem->status=PCB::READY;
		Scheduler::put((PCB*)headBlocked->elem);
		PCBNode * del = headBlocked;
		headBlocked = headBlocked->next;
		delete del;

};

int KernelSem::signal(int n){
	lock;
	if(n>=0){
		if(n==0){
			if(val++<0)
				unblock();
		}
		else{
			int k  = n<numOfBlocked?n:numOfBlocked;
				for(int i = 0; i<k; i++){
					if(val++<0){
						unblock();
					}
				}
				val += n-k;
				n = n<numOfBlocked?n:numOfBlocked;
		}
	}
	unlock;
	return n;
};

int KernelSem::wait(Time maxTimeToWait){
	lock;

	if(val-- <= 0){
		Global::running->status = PCB::BLOCKED;
		Global::running->timeToWait = maxTimeToWait;
		headBlocked=Lists::addInPCBList((PCB*)Global::running, headBlocked);


		unlock;
		dispatch();

		if(Global::running->flagUnblocked==1){
			Global::running->flagUnblocked = 0;
			return 0;
		}

		return 1;
	}

	unlock;
	return 1;
};


int KernelSem::value() const{
	return val;
}


