#pragma once


template <class K, class V>
class node {
public:
	K key;
	V val;
	node<K, V> *p, *l, *r;

	int size;
	node<K, V> *link;

	bool rev;

	node(K key = K(), V val = V(), 
		node<K,V> *l = NULL, node<K,V> *r = NULL, node<K,V> *p = NULL) :
	key(key), val(val), l(l), r(r), p(p) 
	{
		size = 1;
		link = NULL;
		rev  = false;
	}

	/*
		make p-node parent of c-node
		INPUT:
			*c - pointer to child node
			*p - pointer to parent node
	*/
	static void set_parent(node<K,V> *c, node<K,V> *p) {
		if (c) c->p = p;
	}

	/*
		make v-node parent of its childrens
		INPUT:
			*v - pointer to node
	*/
	static void keep_parent(node<K,V> *v) {
		if (!v) return;
		set_parent(v->l, v);
		set_parent(v->r, v);

		recomp(v);
	}

	/*
		left/right rotate of c-node
		INPUT:
			*c - pointer to node
			*p - pointer to parent of c-node
	*/
	static void rotate(node<K,V> *p, node<K,V> *c) {
		node<K,V> *g = p->p;

		if (g) {
			if (p == g->l) g->l = c;
			else		   g->r = c;
		}

		if (c == p->l) p->l = c->r, c->r = p;
		else		   p->r = c->l, c->l = p;

		keep_parent(p);
		keep_parent(c);

		c->p = g;
	}

	/*
		поднимает узел v
		INPUT:
			*v - pointer to node
		OUTPUT:
			pointer to node
	*/
	static node<K,V> *splay(node<K,V> *v) {
		if (!v->p) return v;

		node<K,V> *p = v->p;
		node<K,V> *g = p->p;

		if (!g) {
			rotate(p, v);
			return v;
		}
		else {
			bool zigzig = g->l == p && p->l == v;

			if (zigzig) {
				rotate(g, p);
				rotate(p, v);
			}
			else {
				rotate(p, v);
				rotate(g, v);
			}

			return splay(v);
		}
	}

	
	/*
		перерасчет размера поддерева узла *v
		INPUT:
			*v - pointer to node
	*/
	static void recomp(node<K,V> *v) {
		if (!v) return;
		v->size = 1 + size_of(v->l) + size_of(v->r);
	}

	/*
		size of subtree (numner of nodes in the subtree)
		INPUT:
			*v - pointer to node (subtree)
		OUTPUT:
			size of subtree
	*/
	static int size_of(node<K,V> *v) {
		return v ? v->size : 0;
	}

	/*
		позиция вершины t в in-order обходе дерева, в котором находится t
		(индексация начинается с нуля)
		INPUT:
			*t - pointer to vertex
		OUTPUT:
			position of t in the in-order traversal 
	*/
	static int pos(node<K,V> *t) {
		if (!t) return 0;

		node<K,V> *x = t;

		int i = size_of(x->l) + 1;
		while(x->p) {
			if (x == x->p->r) i += size_of(x->p->l) + 1;
			x = x->p;
		}

		return i-1;
	}

	/*
		returns pointer to left most descendant of vertex v
		OR pointer to itself if there is no left subtree
		INPUT:
			*v - pointer to vertex
		OUTPUT:
			pointer to descendant
	*/
	static node<K,V> *left_most(node<K,V> *v) {
		node<K,V> *cur = v;
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
	static node<K,V> *right_most(node<K,V> *v) {
		node<K,V> *cur = v;
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
	static node<K,V> *before(node<K,V> *v) {
		if (left(v))
			return right_most(left(v));
		else {
			node<K,V> *cur = v;
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
	static node<K,V> *after(node<K,V> *v) {
		if (right(v))
			return left_most(right(v));
		else {
			node<K,V> *cur = v;
			while( cur && cur == right(parent(cur)) )
				cur = parent(cur);
			
			if(cur) cur = parent(cur);
			return cur;
		}
	}
	
	static node<K,V> *left(node<K,V> *v) {
		return v ? v->l : NULL;
	}
	
	static node<K,V> *right(node<K,V> *v) {
		return v ? v->r : NULL;
	}
	
	static node<K,V> *parent(node<K,V> *v) {
		return v ? v->p : NULL;
	}
};
