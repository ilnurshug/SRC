#pragma once

#include "bst_node.h"

template <class ...Types>
class im_treap {
public:
	static node<int,Types...> *at(node<int,Types...> *t, int pos) {
		node<int,Types...> *cur = t;
		while(cur) {
			int size = node<int,Types...>::size_of(cur->l);
			if (pos == size) 
				return cur;

			cur = size > pos ? cur->l : cur->r;
			if (size < pos)
				pos -= size + 1;
		}
		return NULL;
	}

	static void insert (node<int,Types...> * &t, Types... x, int pos) {
        node<int,Types...> *left = NULL, *right = NULL,
					*tmp = new node<int,Types...>(rand());

		tmp->info = std::make_tuple(x...);

        split (t, pos, left, right);

        merge (t, left, tmp);
        merge (t, t, right);
    }

	static void remove(node<int,Types...> * &t, int pos) {
		node<int,Types...> *left = NULL, *right = NULL, *m = NULL;
		pos++;

		split(t, pos, left, right);
		split(left, pos-1, left, m);

		delete m;
		
		merge(t, left, right);
	}

	static void merge (node<int,Types...> * &t, 
					   node<int,Types...> *left, node<int,Types...> *right) 
	{
        if (!left || !right)         
			t = left ? left : right;
        else if (left->key > right->key) 
			merge (left->r, left->r, right),  t = left;
        else                         
			merge (right->l, left, right->l), t = right;

		recalc(t);
		node<int,Types...>::keep_parent(t);
	}

	static void split (node<int,Types...> *t, int size, 
					   node<int,Types...> * &left, node<int,Types...> * &right) 
	{
        if (!t) {
            left = right = NULL;
            return;
        }
        
        int cur = node<int,Types...>::size_of(t->l) + 1;
        if (cur <= size) {                 
            left = t;
            split (left->r, size - cur, left->r, right);
            recalc(left);
			node<int,Types...>::keep_parent(left);
			node<int,Types...>::set_parent(right, NULL);
        }
        else {
            right = t;
            split (right->l, size, left, right->l);
            recalc(right);
			node<int,Types...>::keep_parent(right);
			node<int,Types...>::set_parent(left, NULL);
        }
    }

	static void recalc (node<int,Types...> *t) {
        if (!t) return;

        t->size = node<int,Types...>::size_of(t->l) + node<int,Types...>::size_of(t->r) + 1;

		t->info = tree_update<Types...>::append(t->info, 
												node<int,Types...>::get_info(t->l), 
												node<int,Types...>::get_info(t->r)
												);
    }

	static void update(node<int,Types...> *v) {
		node<int,Types...> *root = v;
		while(root) {
			recalc(root);
			root = root->p;
		}
	}
};
