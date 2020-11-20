/*
 * semaphor.cpp
 *
 *  Created on: Aug 20, 2020
 *      Author: OS1
 */

#include "semaphor.h"
#include "krlsem.h"

#include <iostream.h>

Semaphore::Semaphore(int init) {
	myImpl = new KernelSem(init, this);
}

Semaphore::~Semaphore() {
}

 int Semaphore::signal(int n){

	return myImpl->signal(n);

}

 int Semaphore::wait(Time maxTimeToWait){

	return myImpl->wait(maxTimeToWait);

}

 int Semaphore::val () const{
	 return myImpl->value();
 }
