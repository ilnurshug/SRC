#pragma once

template <class T>
class stack {

	template <class T>
	class node {
	public:
		T x;
		node<T> *nxt;

		node(T x = T(), node<T> *nxt = NULL) : x(x), nxt(nxt) {}
	};

public:

	stack() : sz(0), t(NULL) { }
	~stack() {
		destruct();
	}

	stack& operator = (const stack<T> &s) {
		if (this == &s) return *this;

		destruct();

		node<T> *ptr = s.t, *nxt = NULL, *prev = NULL;
		while(ptr) {
			nxt = new node<T>(ptr->x);

			if (prev) prev->nxt = nxt;
			else	  t = nxt;

			prev = nxt;
			sz++;

			ptr = ptr->nxt;
		}

		return *this;
	}

	void push(T x) {
		node<T> *tmp = new node<T>(x, t);
		t = tmp;
		sz++;
	}

	void pop() {
		if (!t) return;
		
		node<T> *tmp = t;
		t = t->nxt;
		delete tmp;
		sz--;
	}

	T top() {
		return t ? t->x : T();
	}

	int size() const { return sz; }

	bool empty() const { return size() == 0; }

	class iterator;
	friend class iterator;

	iterator begin() { return iterator(t); }

	iterator end() { return iterator(); }

private:

	void destruct() {
		while(t) {
			node<T> *cur = t->nxt;
			delete t;
			t = cur;
		}
		t = NULL;
		sz = 0;
	}

	node<T> *t;

	int sz;
};

template <class T>
class stack<T>::iterator {

	node<T> *it;

	iterator(node<T> *it) {
		this->it = it;
	}

public:
	friend class stack;

	iterator() { it = NULL; }

	T& operator* () {
		return it->x;
	}

	iterator& operator++ () {
		if (it) it = it->nxt;
		return *this;
	}

	bool operator == (const iterator &rhs) { return it == rhs.it; }

	bool operator != (const iterator &rhs) { return it != rhs.it; }
};
