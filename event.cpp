/*
 * Event.cpp
 *
 *  Created on: Aug 22, 2020
 *      Author: OS1
 */

#include "event.h"
#include "krlev.h"

Event::Event(IVTNo ivtNo) {
	myImpl = new KernelEv(ivtNo,this);
}

Event::~Event() {

	delete myImpl;
}

void Event::wait(){
	myImpl->wait();
}

void Event::signal(){
	myImpl->signal();
}

