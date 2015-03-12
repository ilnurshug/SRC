#pragma once

#include "bst_node.h"

template <class K, class V>
class splay_tree {
public:
	/*
		попытка найти узел с ключом key в поддереве v
		INPUT:
			*v  - pointer to subtree
			key - key to find
		OUTPUT:
			указатель на узел с ближайшим к key значением ключа
	*/
	static node<K,V> *find(node<K,V> *v, K key) {
		if (!v) return NULL;

		if      (v->key == key)		   return node<K,V>::splay(v);
		else if (v->l && key < v->key) return find(v->l, key);
		else if (v->r && key > v->key) return find(v->r, key);
		
		return node<K,V>::splay(v);
	}

	/*
		insert to tree new node
		INPUT:
			*root - pointer to tree
			key   - key of new node
			val   - value of new node
	*/
	static void insert(node<K,V> *&root, K key, V val) {
		pair<node<K,V>*, node<K,V>*> lr = split(root, key);

		root = new node<K,V>(key, val, lr.first, lr.second);
		node<K,V>::keep_parent(root);
	}

	/*
		remove node with key from tree
		INPUT:
			*root - pointer to tree
			key   - key of node
	*/
	static void remove(node<K,V> *&root, K key) {
		root = find(root, key);

		node<K,V>::set_parent(root->l, NULL);
		node<K,V>::set_parent(root->r, NULL);

		node<K,V> *tmp = root;

		root = merge(root->l, root->r);		

		// prevent memory leaks 
		tmp->l = NULL, tmp->r = NULL;
		delete tmp;
	}
	
	/*
		split tree into two subtress:
		any key from left subtree less than key, 
		any key from right subtree greater than key.
		INPUT:
			*root - pointer to tree
			key   - devider
		OUTPUT:
			pair<L, R> where L - pointer to left subtree, 
							 R - pointer to right subtree
	*/
	static pair< node<K,V>*, node<K,V>* > split(node<K,V> *root, K key) {
		if (!root) return pair< node<K,V>*, node<K,V>* >(NULL, NULL);

		root = find(root, key);

		if (root->key == key) {
			node<K,V>::set_parent(root->l, NULL);
			node<K,V>::set_parent(root->r, NULL);
			return make_pair(root->l, root->r);
		}
		else if (key < root->key) {
			node<K,V> *l = root->l;
			root->l = NULL;
			node<K,V>::set_parent(l, NULL);

			return make_pair(l, root);
		}
		else {
			node<K,V> *r = root->r;
			root->r = NULL;
			node<K,V>::set_parent(r, NULL);

			return make_pair(root, r);
		}
	}

	/*
		merges two trees into one
		INPUT:
			*l - pointer to left tree
			*r - pointer to right tree
		OUTPUT:
			pomiter to new tree
		REQUIREMENTS:
			all keys in r-tree should be greater than any key from l-tree
	*/
	static node<K,V> *merge(node<K,V> *&l, node<K,V> *&r) {
		if (!l || !r) return l ? l : r;

		r = find(r, l->key);
		r->l = l;
		l->p = r;

		return r;
	}
};