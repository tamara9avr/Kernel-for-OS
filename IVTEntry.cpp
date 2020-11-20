/*
 * IVTEntry.cpp
 *
 *  Created on: Aug 23, 2020
 *      Author: OS1
 */

#include "IVTEntry.h"
#include <dos.h>
#include "krlev.h"

#include "Global.h"

IVTEntry::IVTEntry(IVTNo num, pInterrupt newIntr){

	numEntry = num;

#ifndef BCC_BLOCK_IGNORE
	oldIntr = getvect(numEntry);
	setvect(numEntry,newIntr);
#endif

	myEvent = 0;
	Global::ivtEntries[numEntry] = this;
}

IVTEntry::~IVTEntry() {

	Global::ivtEntries[numEntry] = 0;

#ifndef BCC_BLOCK_IGNORE
	setvect(numEntry,oldIntr);
#endif

}

void IVTEntry::callOld(){
	oldIntr();
}

IVTEntry* IVTEntry::getIVTEntry(IVTNo ivtNo){
	return Global::ivtEntries[ivtNo];
}

void IVTEntry::signal(){
	if(myEvent!=0)
		myEvent->signal();
}
