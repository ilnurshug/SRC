#pragma once
#include "bst_node.h"

template <class T>
class avl_tree {

	template <class T>
	class _avl_tree 
	{
	public:
		static const int AI = 1; // ALLOWED_IMBALANCE

		static node<T,int>* find(node<T,int> *t, T x) {
			if (!t)				 return NULL;
			else if (x < t->key) return find(t->l, x);
			else if(x > t->key)  return find(t->r, x);
			else				 return t;
		}

		static node<T,int>* insert(node<T,int>* &t, T x) {
			if (!t) {
				t = new node<T,int>(x);
				t->info = std::make_tuple(0);
				return t;
			}

			if (x < t->key) {
				t->l = insert(t->l, x);
				node<T,int>::set_parent(t->l, t);
			}
			else if(x > t->key) {
				t->r = insert(t->r, x);
				node<T,int>::set_parent(t->r, t);
			}
			else {
				// equal keys
			}

			return balance(t);
		}

		static node<T,int>* remove(node<T,int>* &t, T x) {
			if(!t) return t;

			if (x < t->key)
				t->l = remove(t->l, x);
			else if(x > t->key)
				t->r = remove(t->r, x);
			else if(t->l && t->r) {
				t->key = node<T,int>::left_most(t->r)->key;
				t->r = remove(t->r, t->key);
			}
			else {
				node<T,int>* c = (t->l != NULL) ? t->l : t->r, *p = t->p; 
				bool left_child = (p && t == p->l);

				t->l = t->r = NULL;
				delete t;
				t = NULL;

				if (c) c->p = p;

				if (p) {
					if (left_child) p->l = c;
					else		    p->r = c;
				}
				
				t = c;
				node<T,int>::keep_parent(t);
			}

			return balance(t);
		}

		static node<T,int>* balance(node<T,int>* &t) {
			if (!t) return t;

			if (get_height(t->l) - get_height(t->r) > AI) {
				if (get_height(t->l->l) >= get_height(t->l->r))
					t = rotate_left(t);
				else 
					t = double_rotate_left(t);
			}
			else if (get_height(t->r) - get_height(t->l) > AI) {
				if (get_height(t->r->r) >= get_height(t->r->l))
					t = rotate_right(t);
				else 
					t = double_rotate_right(t);
			}

			set_height(t, max(get_height(t->l), get_height(t->r)) + 1);
			recalc(t);
			return t;
		}

		static node<T,int>* rotate_left(node<T,int>* k2) {
			node<T,int>* k1 = k2->l;
			k2->l = k1->r;
			k1->r = k2;
			k1->p = k2->p;

			set_height(k2, max(get_height(k2->l), get_height(k2->r)) + 1);
			set_height(k1, max(get_height(k1->l), get_height(k2)) + 1);

			recalc(k2);
			recalc(k1);

			node<T,int>::keep_parent(k2);
			node<T,int>::keep_parent(k1);

			return k1;
		}

		static node<T,int>* rotate_right(node<T,int>* k2) {
			node<T,int>* k1 = k2->r;
			k2->r = k1->l;
			k1->l = k2;
			k1->p = k2->p;

			set_height(k2, max(get_height(k2->l), get_height(k2->r)) + 1);
			set_height(k1, max(get_height(k1->r), get_height(k2)) + 1);

			recalc(k2);
			recalc(k1);

			node<T,int>::keep_parent(k2);
			node<T,int>::keep_parent(k1);

			return k1;
		}

		static node<T,int>* double_rotate_left(node<T,int>* k3) {
			k3->l = rotate_right(k3->l);
			return rotate_left(k3);
		}

		static node<T,int>* double_rotate_right(node<T,int>* k3) {
			k3->r = rotate_left(k3->r);
			return rotate_right(k3);
		}

		static void recalc(node<T,int>* t) {
			t->size = node<T,int>::size_of(t->l) + 
				node<T,int>::size_of(t->r) + 1;
		}

		static int get_height(node<T,int> *t) {
			return !t ? -1 : std::get<0>(t->info);
		}

		static void set_height(node<T,int> *t, int h) {
			std::get<0>(t->info) = h;
		}
	};


public:

	avl_tree() { root = NULL; }
	~avl_tree() { 
		delete root;
		root = NULL;
	}

	class iterator;
	friend class iterator;

	iterator find(T x) { return iterator(_avl_tree<T>::find(root, x)); }

	inline iterator insert(T x) {
		root = _avl_tree<T>::insert(root, x);
		return iterator(root);
	}

	inline void erase(T x) {
		root = _avl_tree<T>::remove(root, x);
	}

	inline void erase(iterator it) {
		erase(it.it->key);
	}

	iterator begin() { return iterator(node<T,int>::left_most(root)); }

	iterator end() { return iterator(0); }

	inline void push_back(T x) {insert(x);}

private:

	node<T,int> *root;
};

template <class T> 
class avl_tree<T>::iterator {

	node<T,int> *it;

	iterator(node<T,int> *it) { this->it = it; }

public:
	friend class avl_tree;

	iterator() { it = NULL; }

	T& operator* () {
		return it->key;
	}

	iterator& operator++ () {
		it = node<T,int>::after(it);
		return *this;
	}

	iterator& operator-- () {
		it = node<T,int>::before(it);
		return *this;
	}

	bool operator== (const iterator &rhs) { return it == rhs.it; }

	bool operator!= (const iterator &rhs) { return it != rhs.it; }
};