#pragma once

#include <tuple>

template <class... Types>
class tree_update {
public:
    static std::tuple<Types...> zero();
    static std::tuple<Types...> append(std::tuple<Types...> &rt, 
									   std::tuple<Types...> &l, 
									   std::tuple<Types...> &r);
};

template <class K, class... I>
class node {
public:
	K key;
	std::tuple<I...> info;
	int size;
	node<K,I...> *l, *r, *p;

	node(K key = K(), 
		node<K,I...> *l = NULL,
		node<K,I...> *r = NULL,
		node<K,I...> *p = NULL) :
	key(key), l(l), r(r), p(p), size(1)
	{}

	~node() {
		delete l;
		delete r;
		l = r = NULL;
	}
	
	static node<K,I...> *left(node<K,I...> *v) {
		return v ? v->l : NULL;
	}
	
	static node<K,I...> *right(node<K,I...> *v) {
		return v ? v->r : NULL;
	}
	
	static node<K,I...> *parent(node<K,I...> *v) {
		return v ? v->p : NULL;
	}

	/*
		returns pointer to left most descendant of vertex v
		OR pointer to itself if there is no left subtree
		INPUT:
			*v - pointer to vertex
		OUTPUT:
			pointer to descendant
	*/
	static node<K,I...> *left_most(node<K,I...> *v) {
		node<K,I...> *cur = v;
		while(left(cur)) 
			cur = left(cur);
		return cur;
	}
	
	/*
		returns pointer to right most descendant of vertex v 
		OR pointer to itself if there is no right subtree
		INPUT:
			*v - pointer to vertex
		OUTPUT:
			pointer to descendant 
	*/
	static node<K,I...> *right_most(node<K,I...> *v) {
		node<K,I...> *cur = v;
		while(right(cur)) 
			cur = right(cur);
		return cur;
	}
	
	/*
		возвращает указатель на вершину, которая находится перед 
		вершиной v в in-order обходе дерева
		INPUT:
			*v - pointer to vertex
		OUTPUT:
			pointer to vertex u : in-order traversal = (..., u, v, ...)
	*/
	static node<K,I...> *before(node<K,I...> *v) {
		if (left(v))
			return right_most(left(v));
		else {
			node<K,I...> *cur = v;
			while( cur && cur == left(parent(cur)) )
				cur = parent(cur);
			
			if(cur) cur = parent(cur);
			return cur;
		}
	}
	
	/*
		возвращает указатель на вершину, которая находится после 
		вершины v в in-order обходе дерева
		INPUT:
			*v - pointer to vertex
		OUTPUT:
			pointer to vertex u : in-order traversal = (..., v, u, ...)
	*/
	static node<K,I...> *after(node<K,I...> *v) {
		if (right(v))
			return left_most(right(v));
		else {
			node<K,I...> *cur = v;
			while( cur && cur == right(parent(cur)) )
				cur = parent(cur);
			
			if(cur) cur = parent(cur);
			return cur;
		}
	}
	
	/*
		позиция вершины t в in-order обходе дерева, в котором находится t
		(индексация начинается с нуля)
		INPUT:
			*t - pointer to vertex
		OUTPUT:
			position of t in the in-order traversal 
	*/
	static int pos(node<K,I...> *t) {
		if (!t) return 0;

		node<K,I...> *x = t;

		int i = size_of(x->l) + 1;
		while(x->p) {
			if (x == x->p->r) i += size_of(x->p->l) + 1;
			x = x->p;
		}

		return i-1;
	}

	static void set_parent(node<K,I...> *c, node<K,I...> *p) {
		if (c) c->p = p;
	}

	static void keep_parent(node<K,I...> *t) {
		if (!t) return;
		set_parent(t->l, t);
		set_parent(t->r, t);
	}

	static int size_of (node<K,I...> *t) { 
		return t ? t->size : 0; 
	}

	static std::tuple<I...> get_info(node<K,I...> *t) {
		return t ? t->info : tree_update<I...>::zero();
	}

	static node<K,I...> *find_root(node<K,I...> *v) {
		if (!v) return NULL;

		node<K,I...> *root = v;
		while(root->p) 
			root = root->p;
		return root;
	}
};