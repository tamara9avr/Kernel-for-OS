/*
 * Lists.cpp
 *
 *  Created on: Sep 7, 2020
 *      Author: OS1
 */

#include "Lists.h"
#include "Global.h"
#include "krlsem.h"
#include "PCB.h"

PCBNode* Lists::addInPCBList(PCB* pcb, PCBNode* head){
	if(head == 0){
		lock;
		head = new PCBNode();
		head->elem = pcb;
		head->next = 0;
		unlock;
	}
	else{
		PCBNode* tail = head;
		while(tail->next!=0) tail=tail->next;

		lock;
		tail->next = new PCBNode();
		tail->next->elem = pcb;
		tail->next->next = 0;
		tail = tail->next;
		unlock;

	}

	return head;
}

PCBNode* Lists::deletePCBList(PCBNode * head){
	lock;
	if(head == 0) return 0;
	while(head!=0){
		PCBNode *del = head;
		head = head->next;
		delete del;

	}
	unlock;
	return 0;
}

PCBNode* Lists::deletePCBListElem(PCB* pcb, PCBNode* head){

	if(head != 0){

		PCBNode* curr = head, *prev=0;

		while(curr!=0 && curr->elem!=pcb){
			prev = curr;
			curr=curr->next;
		}

		if(curr->elem==pcb){

			if(curr==head){
				head = head->next;
				lock;
				delete curr;
				delete prev;
				unlock;
			}
			else{
				prev->next = curr->next;
				lock;
				delete curr;
				unlock;
			}
		}
		else {
			lock;
			delete curr;
			delete prev;
			unlock;
		}
	}
	return head;

}

SemNode* Lists::addInSemList(KernelSem* ksem, SemNode* head){
	if(head == 0){
		lock;
		head = new SemNode();
		head->elem = ksem;
		head->next = 0;
		unlock;
	}
	else{
		SemNode* tail = head;
		while(tail->next!=0) tail=tail->next;

		lock;
		tail->next = new SemNode();
		tail->next->elem = ksem;
		tail->next->next = 0;
		tail = tail->next;
		unlock;
	}

	return head;
}

SemNode* Lists::deleteSemList(SemNode* head){
	lock;
	if(head == 0) return 0;
	while(head!=0){
		SemNode *del = head;
		head = head->next;
		delete del;

	}
	unlock;
	return 0;
}

SemNode* Lists::deleteSemListElem(KernelSem* sem, SemNode* head){
	if(head != 0){

		SemNode* curr = head, *prev=0;

		while(curr!=0 && curr->elem!=sem){
			prev = curr;
			curr=curr->next;
		}

		if(curr->elem==sem){

			if(curr==head){
				head = head->next;
				lock;
				delete curr;
				delete prev;
				unlock;
			}
			else{
				prev->next = curr->next;
				lock;
				delete curr;
				unlock;
			}
		}
		else {
			lock;
			delete curr;
			delete prev;
			unlock;
		}
	}
	return head;
}

#ifdef KERNEL_SIGNAL

HandlerNode* Lists::addHandler(SignalHandler sh, HandlerNode* head){
	if(head == 0){
		lock;
		head = new HandlerNode();
		head->handler = sh;
		head->next = 0;
		unlock;
	}
	else{
		HandlerNode* tail = head;
		while(tail->next!=0) tail=tail->next;

		lock;
		tail->next = new HandlerNode();
		tail->next->handler = sh;
		tail->next->next = 0;
		tail = tail->next;
		unlock;

	}

	return head;
}

HandlerNode* Lists::deleteHandlerList(HandlerNode * head){
	lock;
	if(head == 0) return 0;
	while(head!=0){
		HandlerNode *del = head;
		head = head->next;
		delete del;

	}
	unlock;
	return 0;
}

HandlerNode* Lists::deleteHandlerElem(SignalHandler sh, HandlerNode* head){

	if(head != 0){

		HandlerNode* curr = head, *prev=0;

		while(curr!=0 && curr->handler!=sh){
			prev = curr;
			curr=curr->next;
		}

		if(curr->handler==sh){

			if(curr==head){
				head = head->next;
				lock;
				delete curr;
				delete prev;
				unlock;
			}
			else{
				prev->next = curr->next;
				lock;
				delete curr;
				unlock;
			}
		}
		else {
			lock;
			delete curr;
			delete prev;
			unlock;
		}
	}
	return head;

}


HandlerNode* Lists::swapHandlers(SignalHandler s1, SignalHandler s2, HandlerNode* head){
	HandlerNode* curr1 = head, *curr2 = head;
	while((curr1!=0 && curr1->handler!= s1) || (curr2!=0 && curr2->handler!=s2)){
		if(curr1!=0 && curr1->handler!=s1) curr1 = curr1->next;
		if(curr2!=0 && curr2->handler!=s2) curr2 = curr2->next;
	}

	if(curr1 == 0 || curr2 == 0) return head;

	else{
		curr1->handler = s2;
		curr2->handler = s1;
	}

	return head;
}

SignalNode* Lists::addInSignalList(SignalId sh, SignalNode* head){
	if(head == 0){
		lock;
		head = new SignalNode();
		head->signal = sh;
		head->next = 0;
		unlock;
	}
	else{
		SignalNode* tail = head;
		while(tail->next!=0) tail=tail->next;

		lock;
		tail->next = new SignalNode();
		tail->next->signal = sh;
		tail->next->next = 0;
		tail = tail->next;
		unlock;

	}

	return head;
}

SignalNode* Lists::deleteSignalList(SignalNode * head){
	lock;
	if(head == 0) return 0;
	while(head!=0){
		SignalNode *del = head;
		head = head->next;
		delete del;

	}
	unlock;
	return 0;
}

SignalNode* Lists::deleteSignalListElem(SignalId sh, SignalNode* head){

	if(head != 0){

		SignalNode* curr = head, *prev=0;

		while(curr!=0 && curr->signal!=sh){
			prev = curr;
			curr=curr->next;
		}

		if(curr->signal==sh){

			if(curr==head){
				head = head->next;
				lock;
				delete curr;
				delete prev;
				unlock;
			}
			else{
				prev->next = curr->next;
				lock;
				delete curr;
				unlock;
			}
		}
		else {
			lock;
			delete curr;
			delete prev;
			unlock;
		}
	}
	return head;

}


#endif




















