/*
 * Thread.h
 *
 *  Created on: Aug 18, 2020
 *      Author: OS1
 */

#ifndef _THREAD_H_
#define _THREAD_H_


typedef unsigned long StackSize;
typedef unsigned int Time; // time, x 55ms
const StackSize defaultStackSize = 4096;
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;

#define KERNEL_SIGNAL

#ifdef KERNEL_SIGNAL

typedef void (*SignalHandler)();
typedef unsigned SignalId;


#endif

class PCB;

class Thread {
public:

	 void start();

	 void waitToComplete();

	 virtual ~Thread();

	 ID getId();

	 static ID getRunningId();

	 static Thread * getThreadById(ID id);

#ifdef KERNEL_SIGNAL

	 void signal(SignalId signal);

	void registerHandler(SignalId signal, SignalHandler handler);

	void unregisterAllHandlers(SignalId id);

	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal(SignalId signal);

	static void blockSignalGlobally(SignalId signal);

	void unblockSignal(SignalId signal);

	static void unblockSignalGlobally(SignalId signal);

#endif

protected:

	 friend class PCB;

	 friend class Global;

	 friend void dispatch();

	 Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);

	 virtual void run();

private:

	 PCB* myPCB;

};

#endif /* _THREAD_H_ */
