#pragma once

#include "im_splay_tree.h"


template <class T>
class rope {
public:
	
	class iterator;
	friend class iterator;
	
	rope() { root = NULL; }
	//~rope();
	
	/*
		делит исходную строку src по итератору it
		на две части
		INPUT:
			src   - строка
			it    - позиция в строке src
		OUTPUT:
			left  - содержит src[...it)
			right - содержит src[it...]
		WARNING:
			процедура уничтожает src
	*/
	static void split(rope<T> &src, iterator it,
					  rope<T> &left, rope<T> &right)
	{
		im_splay_tree<int,T>::cleanup(it.ptr);				// src.root
		src.root = node<int,T>::splay(it.ptr);
		
		pair<node<int,T>*,node<int,T>*> 
			lr = im_splay_tree<int,T>::split(src.root, node<int,T>::size_of(src.root->l) + 1);
			
		left.root = lr.first;
		right.root = lr.second;
		
		src.root = NULL;
	}

	/*
		склеивает строки lhs и rhs, результат помещает в dst
		INPUT:
			lhs/rhs - левая и правая строка
		OUTPUT:
			dst - содержит результирующую строку
		WARNING:
			процедура уничтожает lhs и rhs
	*/
	static void concat(rope<T> &dst, rope<T> &lhs, rope<T> &rhs) {
		dst.root = im_splay_tree<int,T>::merge(lhs.root, rhs.root);
		lhs.root = rhs.root = NULL;
	}
	
	static void get_rope(rope<T> &r, iterator it) {
		im_splay_tree<int,T>::cleanup(it.ptr);
		r.root = node<int,T>::splay(it.ptr);
	}
	
	static void reverse(rope<T> &r) {
		im_splay_tree<int,T>::reverse(r.root);
	}

	int size() const {
		return node<int,T>::size_of(root);
	}

	bool empty() const {
		return size() == 0;
	}

	void push_back(T x) {
		node<int,T> *b = new node<int,T>(0, x);
		root = im_splay_tree<int,T>::merge(root, b);
	}

	iterator begin() {
		root = im_splay_tree<int,T>::find(root, 0);
		return iterator(root);
	}
	
	iterator rbegin() {
		root = im_splay_tree<int,T>::find(root, node<int,T>::size_of(root)-1);
		return iterator(root);
	}
	
	iterator end() {
		return iterator(NULL);
	}
	
	iterator erase(iterator it) {
		// TODO
		return iterator(NULL);
	}
	
private:
	
	node<int, T> *root;
};

template <class T>
class rope<T>::iterator {
	node<int,T> *ptr;

	iterator(node<int,T> *ptr) { this->ptr = ptr; }
	
public:
	friend class rope;

	iterator() { ptr = NULL; }

	iterator & operator = (const iterator& it) {
		if (this == &it) return *this;

		ptr = it.ptr;
		im_splay_tree<int,T>::cleanup(ptr);
		ptr = node<int,T>::splay(ptr);

		return *this;
	}

	T& operator* () {
		return ptr->val;
	}

	iterator& operator++ () {
		ptr = shift(1).ptr;
		return *this;
	}

	iterator& operator-- () {
		ptr = shift(-1).ptr;
		return *this;
	}

	iterator operator + (const int& i) {
		iterator it = shift(i);
		
		im_splay_tree<int,T>::cleanup(ptr);
		ptr = node<int,T>::splay(ptr);
		return it;
	}

	iterator operator - (const int& i) {
		iterator it = shift(-i);
		
		im_splay_tree<int,T>::cleanup(ptr);
		ptr = node<int,T>::splay(ptr);
		return it;
	}

	bool operator == (const iterator &rhs) { return ptr == rhs.ptr; }

	bool operator != (const iterator &rhs) { return ptr != rhs.ptr; }
	
private:

	iterator shift(const int& i) {
		im_splay_tree<int,T>::cleanup(ptr);
		ptr = node<int,T>::splay(ptr);

		int pos = node<int,T>::pos(ptr) + i;
		if (pos >= node<int,T>::size_of(ptr) || pos < 0)
			return iterator(NULL);
		else 
			return iterator(im_splay_tree<int,T>::find(ptr, pos));
	}

};