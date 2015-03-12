#pragma once

#include "im_splay_tree.h"


template <class K, class V>
class link_cut_tree {
public:

	/*
		Makes vertex v a new child of vertex w, i.e. adds an edge (v,w).
		INPUT:
			*v - pointer to the root
			*w - pointer to some node in other tree
		OUTPUT:
			pointer to the new tree
		REQUIREMENTS:
			v is the root of its represented tree, 
			v and w are nodes of distinct trees
	*/
	static node<K,V> *link(node<K,V> *v, node<K,V> *w) {
		v->link = w;
		return expose(v);
	}

	/*
		Deletes the edge between vertex c and its parent p
		INPUT:
			*c - pointer to the node
		REQUIREMENTS:
			c is not the root
	*/
	static void cut(node<K,V> *c) {
		expose(parent(c));
		c->link = NULL;
	}

	/*
		Returns the root of the tree that contains vertex v
		INPUT:
			*v - pointer to the node
		OUTPUT:
			pointer to the root
	*/
	static node<K,V> *find_root(node<K,V> *v) {
		return expose(v);
	}

	/*
		returns pointer to parent of vertex v in the represented tree (R)
		INPUT:
			*v - pointer to vertex
		OUTPUT:
			pointer to parent
	*/
	static node<K,V> *parent(node<K,V> *v) {
		expose(v);
		return node<K,V>::before(v);
	}

	/*
		returns depth of the node in the represented tree (R)
		(number of edges on the root-v path)
		INPUT:
			*v - pointer to node
		OUTPUT:
			depth of the node
	*/
	static int depth(node<K,V> *v) {
		expose(v);
		return node<K,V>::size_of(support_root(v)) - 1;
	}

	/*
		least common ancestor of vertices v and u
		INPUT:
			*v, *u - pointers to vertices
		OUTPUT:
			pointer to lca or NULL if v and u are in the different trees
	*/
	static node<K,V> *lca(node<K,V> *v, node<K,V> *u) {
		if (depth(v) < depth(u)) swap(v, u);

		expose(v);
		expose(u);
		return im_splay_tree<K,V>::find(support_root(v), 0)->link;
	}

	/*
		add edge between vertices v and u
		INPUT:
			*v,*u - pointers to nodes
		REQUIREMENTS:
			v and u are nodes of distinct represented trees
	*/
	static void add_edge(node<K,V> *v, node<K,V> *u) {
		evert(v);
		link(v, u);
	}
	
	/*
		remove edge between vertices v and u
		INPUT:
			*v,*u - pointers to nodes
		REQUIREMENTS:
			v and u are nodes of same represented tree
	*/
	static void remove_edge(node<K,V> *u, node<K,V> *v) {
		evert(v);
		cut(u, v);
	}

	/*
		modify the represented tree containing vertex v by making v root.
		(This operation can be regarded as reversing the direction of every edge 
		on the path from v to the original root.)
		INPUT:
			*v - pointer to the node
	*/
	static void evert(node<K,V> *v) {
		node<K,V> *root = expose(v);
		im_splay_tree<K,V>::reverse(support_root(root));
	}

private:
	/*
		auxiliary trees
	*/

	/*
		A preferred path from the root of R(represented tree) down to v 
		is formed. When this preferred path is formed every edge on the path 
		becomes preferred and all the old preferred edges in R that 
		had an endpoint on this path are destroyed, and replaced by 
		path-parent pointers (link).
		INPUT:
			*v - pointer to node
		OUTPUT:
			pointer to the root of represented tree (R)
	*/
	static node<K,V> *expose(node<K,V> *v) {
		v = im_splay_tree<K,V>::find(cut_out(v), 0);
		
		while(v->link) {
			node<K,V> *nxt = cut_out(v->link);
			v->link = NULL;
			v = im_splay_tree<K,V>::find(im_splay_tree<K,V>::merge(nxt, v), 0);
		}
		return v;
	}

	/*
		separate all the nodes less than v (lower in the prefered-path) 
		in a separate auxiliary tree
		INPUT:
			*v - pointer to node
	*/
	static node<K, V> *cut_out(node<K,V> *v) {
		support_root(v);
		std::pair<node<K,V>*,node<K,V>*> 
			lr = im_splay_tree<K,V>::split(v, node<K,V>::size_of(v->l) + 1);
		
		if (lr.second)
			im_splay_tree<K,V>::find(lr.second, 0)->link = v;
		return v;
	}

	static void cleanup(node<K,V> *v) {
		if (!v) return;
		cleanup(v->p);
		im_splay_tree<K,V>::push(v);
	}

	static node<K,V> *support_root(node<K,V> *v) {
		cleanup(v);
		return node<K,V>::splay(v);
	}
};