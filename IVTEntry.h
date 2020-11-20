/*
 * IVTEntry.h
 *
 *  Created on: Aug 23, 2020
 *      Author: OS1
 */

#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_


typedef void interrupt (*pInterrupt)(...);

typedef unsigned char IVTNo;

class KernelEv;

class IVTEntry {
public:

	IVTEntry(IVTNo num, pInterrupt newIntr);

	virtual ~IVTEntry();

	void callOld();

	static IVTEntry* getIVTEntry(IVTNo ivtNo);

	void signal();

protected:

	friend class KernelEv;

private:

	KernelEv* myEvent;

	pInterrupt oldIntr;

	IVTNo numEntry;

};

#endif /* IVTENTRY_H_ */
