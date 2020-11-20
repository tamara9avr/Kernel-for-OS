
#include "Global.h"
#include <iostream.h>
#include "user.h"

struct Integer{
	int num;
};

struct Node {
	Integer* elem;
	Node* next;
};

Node* add(Integer* num, Node* head){
	if(head == 0){
		head = new Node();
		head->elem = num;
		head->next = 0;
	}
	else{
		Node* tail = head;
		while(tail->next!=0) tail=tail->next;

		tail->next = new Node();
		tail->next->elem = num;
		tail->next->next = 0;
		tail = tail->next;
	}

	return head;
}

Node* remove(Integer* num, Node* head){
	if(head != 0){

		Node* curr = head, *prev=0;

		while(curr!=0 && curr->elem!=num){
			prev = curr;
			curr=curr->next;
		}

		if(curr->elem==num){

			if(curr==head){
				head = head->next;
				delete curr;
				delete prev;
			}
			else{
				prev->next = curr->next;
				delete curr;
			}
		}
		else {
			delete curr;
			delete prev;
		}
	}
	return head;
}

Node* deleteList(Node* head){
	if(head == 0) return 0;
	while(head!=0){
		Node *del = head;
		head = head->next;
		cout<<"Deleted "<<del->elem<<endl;
		cout<<"Head "<<head->elem<<endl;
		delete del;

	}
	return 0;
}


int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]){
	Global::init();
	int ret = userMain(argc,argv);
	Global::restore();
	return ret;



/*	Node* head = 0;

	Integer* num7 = new Integer();
	Integer* num8 = new Integer();
	Integer* num9 = new Integer();
	Integer* num10 = new Integer();

	num7->num=7;
	num8->num = 8;
	num9->num = 9;
	num10->num = 10;

	head = add(num7, head);
	head = add(num8,head);
	head = add(num9,head);
	head = add(num10,head);

	head = remove(num8,head);

//	head = deleteList(head);

	while(head!=0){
		cout<<head->elem->num<<endl;
		head = head->next;
	}

	cout<<num8->num<<endl;
*/

}
