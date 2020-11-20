#ifndef _PCBC_H_
#define _PCBC_H_

#include "Thread.h"
#include "Global.h"
#include "Lists.h"

struct PCBNode;

class PCB {

public:

	void start();

	void waitToComplete();

	virtual ~PCB();

	static void wrapper();

	ID getID() const;

	enum Status{ACTIVE,READY,BLOCKED,FINISHED,CREATED};

	Status status;


#ifdef KERNEL_SIGNAL

	void signal(SignalId signal);

	void registerHandler(SignalId signal, SignalHandler handler);

	void unregisterAllHandlers(SignalId id);

	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal(SignalId signal);

	static void blockSignalGlobally(SignalId signal);

	void unblockSignal(SignalId signal);

	static void unblockSignalGlobally(SignalId signal);

	void executeSignals();

#endif

protected:

#ifdef KERNEL_SIGNAL

	friend void signal0();

	PCB* parentThread;

	Signal signals[16];
	static unsigned globallyBlocked[16];

	SignalNode *waiting;


#endif

	PCB(StackSize stackSize, Time timeSlice, Thread* thread);

	friend class Thread;
	friend class Global;
	friend class KernelSem;
	friend void unblockTimeThreads(KernelSem*);

private:

	unsigned sp;
	unsigned ss;
	unsigned bp;
	unsigned* stack;

	int activated;								//Flag activated is used for start method to forbid calling it twice
	int flagUnblocked;
	int flagNoTimer;

	Thread *myThread;

	Time timeSlice;
	Time timeToWait;
	StackSize stackSize;

	PCBNode *head;						//Linked list for all pcbs that are waiting on this one to finish

	ID id;
	static ID cnt;

};

#endif
