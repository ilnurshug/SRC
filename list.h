#pragma once

#include "container.h"

template <class T>
class list : _container<T> {

	template <class T>
	class link {
	public:
		T x;
		link<T> *nxt, *prev;

		link(T x = T(), link<T> *nxt = NULL, link<T> *prev = NULL) :
			x(x), nxt(nxt), prev(prev) {}

		~link() {
			delete nxt;
		}
	};

public:

	list() : _size(0) { 
		head = tail = NULL;
	}
	~list() { 
		destruct(); 
	}

	list(const list<T> &L) {
		_size = 0;
		head = tail = NULL;
		copy(L);
	}

	list & operator = (const list<T> &L) {
    	if (this == &L) return *this;
		
		destruct();	
		copy(L);
		
		return *this;
	}

	int size() const { return _size; }
	bool empty() const { return _size == 0; }

	T front() const { return head->x; }
	T back() const { return tail->x; }

	void push_back(T x) {
		link<T> *tmp = new link<T>(x);
		if (!head) {
			head = tail = tmp;
		}
		else {
			tmp->prev = tail;
			tail->nxt = tmp;
			tail = tmp;
		}
		_size++;
	}

	void push_front(T x) {
		link<T> *tmp = new link<T>(x);
		if (!head) {
			head = tail = tmp;
		}
		else {
			head->prev = tmp;
			tmp->nxt = head;
			head = tmp;
		}
		_size++;
	}

	void pop_front() {
		link<T> *tmp = head;

		head = head->nxt;
		if(head) head->prev = NULL;
		
		remove_subroutine(tmp);
	}

	void pop_back() {
		link<T> *tmp = tail;
		
		tail = tail->prev;
		if(tail) tail->nxt = NULL;
		
		remove_subroutine(tmp);
	}

	class iterator;
	friend class iterator;

	iterator begin() const {
		return iterator(head);
	}

	iterator rbegin() const {
		return iterator(tail);
	}

	iterator end() const {
		return iterator(NULL);
	}

	iterator erase(iterator it) {
		link<T> *nxt = NULL;
		if (it.it == head) {
			pop_front();
			nxt = head;
		}
		else if (it.it == tail) {
			pop_back();
			nxt = tail;
		}
		else {
			nxt = erase(it.it);
		}
		return iterator(nxt);
	}

	iterator insert(T x) { 
		push_front(x); 
		return begin();
	}

private:

	link<T>* erase(link<T> *&v) {
		if (!v) return NULL;
		
		if (v->prev) v->prev->nxt = v->nxt;
		if (v->nxt) v->nxt->prev = v->prev;

		link<T> *nxt = v->nxt;

		remove_subroutine(v);
		return nxt;
	}

	void remove_subroutine(link<T> *&v) {
		_size--;
		if (empty()) head = tail = NULL;
		v->nxt = NULL;
		delete v;
	}

	void destruct() {
		link<T> *ptr = NULL;
		while(head) {
			ptr = head;
			head = head->nxt;

			ptr->nxt = NULL;
			delete ptr;
		}
		head = tail = NULL;
		_size = 0;
	}

	void copy(const list<T> &L) {
		link<T>* cur = L.head;
		for(int i = 0; i < L.size(); i++) {
			push_back(cur->x);
			cur = cur->nxt;	
		}
	}

	int _size;
	link<T> *head, *tail;
};

template <class T>
class list<T>::iterator {
private:
	link<T> *it;

	iterator(link<T> *ptr) {
		it = ptr;
	}

public:
	friend class list;

	iterator() { it = NULL; }

	T& operator* () {
		return it->x;
	}

	iterator& operator++ () {
		if (it) it = it->nxt;
		return *this;
	}

	iterator& operator-- () {
		if (it) it = it->prev;
		return *this;
	}

	bool operator == (const iterator &rhs) { return it == rhs.it; }

	bool operator != (const iterator &rhs) { return it != rhs.it; }
};
