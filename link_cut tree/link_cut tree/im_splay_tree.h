#pragma once

#include "bst_node.h"

template <class K, class V>
class im_splay_tree {
public:
	/*
		найти узел, который находится на позиции lhs в in-order обходе дерева
		INPUT:
			*v  - pointer to subtree
			lhs - position in the in-order traversal of subtree *v
		OUTPUT:
			pointer to node
	*/
	static node<K,V> *find(node<K,V> *v, int lhs) {
		if (!v) return NULL;

		push(v);

		int threshold = node<K,V>::size_of(v->l);

		if (lhs == threshold)		 return node<K,V>::splay(v);
		if (v->l && lhs < threshold) return find(v->l, lhs);
		if (v->r && lhs > threshold) return find(v->r, lhs - threshold - 1);

		return node<K,V>::splay(v);
	}
	
	/*
		разбить дерево root на два, таких что
		левое содержит lhs элементов(узлов), а правое - остальные
		INPUT:
			*root - pointer to tree
			lhs   - size of left subtree
		OUTPUT:
			pair<L,R> where L - pointer to left subtree
							R - pointer to right subtree
	*/
	static std::pair< node<K,V>*, node<K,V>* > split(node<K,V> *root, int lhs) {
		if (!root) 
			return std::pair< node<K,V>*, node<K,V>* >(NULL, NULL);

		root = find(root, lhs);
		if (node<K,V>::size_of(root) <= lhs) 
			return std::pair< node<K,V>*, node<K,V>* >(root, NULL);

		node<K,V>* l = root->l;
		node<K,V>::set_parent(l, NULL);
		root->l = NULL;
		node<K,V>::recomp(root);

		return std::make_pair(l, root);
	}

	/*
		объединяет два дерева по неявному ключу в одно,
		при in-order обходе дерева, элементы дерева l окажутся первыми
		INPUT:
			*l - pointer to left subtree
			*r - pointer to right subtree
		OUTPUT:
			pointer to new tree
	*/
	static node<K,V> *merge(node<K,V> *&l, node<K,V> *&r) {
		if (!l || !r)
			return l ? l : r;

		r = find(r, 0);
		r->l = l;
		node<K,V>::keep_parent(r);
		return r;
	}

	/*
		проталкивание запроса разворота на отрезке в дочерние поддеревья
		INPUT:
			*v - pointer to the tree
	*/
	static void push(node<K,V> *v) {
		if (v && v->rev) {
			swap(v->l, v->r);
			reverse(v->l);
			reverse(v->r);
			v->rev = false;
		}
	}

	static void reverse(node<K,V> *v) {
		if (!v) return;
		v->rev ^= true;
	}

	static void cleanup(node<K,V> *v) {
		if (!v) return;
		cleanup(v->p);
		push(v);
	}
};
