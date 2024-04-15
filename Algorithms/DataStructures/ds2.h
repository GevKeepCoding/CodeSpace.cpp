#pragma once
class MyLinkedList {
	struct node { int val; node* next; node* prev; };

	node* head = nullptr;
	node* tail = nullptr;
	int size = 0;
public:
	MyLinkedList() {

	}

	int get(int index) {
		if (index < 0 || index >= size) return -1;
		node* cur = head;
		for (int i = 0; i < index; i++) cur = cur->next;
		return cur->val;
	}

	void addAtHead(int val) {
		node* newHead = new node{val, head, nullptr};
		if (head) head->prev = newHead;
		head = newHead;
		if (size == 0) tail = head;
		size++;
	}

	void addAtTail(int val) {
		node* newTail = new node{val, nullptr, tail};
		if (tail) tail->next = newTail;
		tail = newTail;
		if (size == 0) head = tail;
		size++;
	}

	void addAtIndex(int index, int val) {
		if (index < 0 || index > size) return;
		if (index == 0) addAtHead(val);
		else if (index == size) addAtTail(val);
		else {
			node* cur = head;
			for (int i = 0; i < index; i++) cur = cur->next;
			node* newNode = new node{val, cur, cur->prev};
			cur->prev->next = newNode;
			cur->prev = newNode;
			size++;
		}
	}

	void deleteAtIndex(int index) {
		if (index < 0 || index >= size) return;
		node* cur = head;
		for (int i = 0; i < index; i++) cur = cur->next;
		if (cur->prev) cur->prev->next = cur->next;
		if (cur->next) cur->next->prev = cur->prev;
		if (cur == head) head = cur->next;
		if (cur == tail) tail = cur->prev;
		delete cur;
		size--;
	}
};
