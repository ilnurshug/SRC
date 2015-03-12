// link_cut tree.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <vld.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "im_splay_tree.h"
using namespace std;


//------------------------------------------------------------------
//
//				NEW IMPLEMENTATION OF LINK-CUT TREE
//
//------------------------------------------------------------------

#include <unordered_map>
#include "rope.h"

class LC_tree {
public:
	
#define NIL -((int)1e9 + 1)

	/*LC_tree() {}
	~LC_tree() {
		
	}*/

	void make_tree(int v) {
		dparent(v) = NIL;
		
		path p;
		p.push_back(v);
		vertex[v] = p.begin();
	}	
	
	/*
		Makes vertex v a new child of vertex w, i.e. adds an edge (v,w).
		INPUT:
			v - root
			w - some node in other tree
		OUTPUT:
			root of the new tree
		REQUIREMENTS:
			v is the root of its represented tree, 
			v and w are nodes of distinct trees
	*/
	int link(int v, int u) {
		dparent(v) = u;
		return expose(v);
	}
	
	/*
		Deletes the edge between vertex c and its parent p
		INPUT:
			c - node
		WARNING:
			если с - корень, то ничего не произойдет
	*/
	void cut(int c) {
		int p = parent(c);
		if (p != c && p != NIL) {
			expose(p);
			dparent(c) = NIL;
		}
	}
	
	/*
		Returns parent of vertex v in the represented tree (R)
		INPUT:
			v - node
		OUTPUT:
			parent of node v
		REQUIREMENTS:
			v is not the root
	*/
	int parent(int v) {
		int root = expose(v);
		if (v == root) return NIL;
		else           return after(v);
	}

	/*
		Returns the root of the tree that contains vertex v
		INPUT:
			v - node
		OUTPUT:
			pointer to the root
	*/
	int find_root(int v) {
		return expose(v);
	}

	/*
		returns depth of the node in the represented tree (R)
		(number of edges on the root-v path)
		INPUT:
			v - node
		OUTPUT:
			depth of the node
	*/
	int depth(int v) {
		expose(v);
		return get_path(v).size()-1;
	}
	
	/*
		least common ancestor of vertices v and u
		INPUT:
			v, u - vertices
		OUTPUT:
			lca or NULL if v and u are in the different trees
	*/
	int lca(int v, int u) {
		if (depth(v) < depth(u)) swap(v, u);

		expose(v);
		expose(u);
		return dparent(tail(get_path(v)));
	}

	/*
		add edge between vertices v and u
		INPUT:
			v,u - nodes
		REQUIREMENTS:
			v and u are nodes of distinct represented trees
	*/
	void add_edge(int v, int u) {
		evert(v);
		link(v, u);
	}
	
	/*
		remove edge between vertices v and u
		INPUT:
			v,u - nodes
		REQUIREMENTS:
			v and u are nodes of same represented tree
	*/
	void remove_edge(int v, int u) {
		evert(v);
		cut(u);
	}

	/*
		modify the represented tree containing vertex v by making v root.
		(This operation can be regarded as reversing the direction of every edge 
		on the path from v to the original root.)
		INPUT:
			v - node
	*/
	void evert(int v) {
		int root = expose(v);
		path::reverse(get_path(root));
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
			v - node
		OUTPUT:
			the root of represented tree (R)
	*/
	int expose(int v) {
		v = tail(get_path(cut_out(v)));
		
		while (dparent(v) != NIL) {
			int nxt = cut_out(dparent(v));
			dparent(v) = NIL;
			
			path res;
			concat(res, get_path(nxt), get_path(v));
			v = tail(res);
		}
		return v;
	}
	
	/*
		separate all the nodes less than v (lower in the prefered-path) 
		in a separate auxiliary tree
		INPUT:
			v - node
	*/
	int cut_out(int v) {
		path p, q;
		
		split(v, p, q);
		
		if (!q.empty()) 
			dparent(tail(q)) = v;
			
		return v;
	}

	typedef rope<int> path;

	inline path get_path(int v) {
		path p;
		path::get_rope(p, vertex[v]);
		return p;
	}

	inline int head(path &p) {
		return *p.rbegin();
	}
	
	inline int tail(path &p) {
		return *p.begin();
	}
	
	inline int before(int v) {
		return *(vertex[v]+1);
	}
	
	inline int after(int v) {
		return *(vertex[v]-1);
	}
	
	inline void concat(path &res, path &p, path &q) {
		int p_id = tail(p), q_id = tail(q);
	
		path::concat(res, p, q);
	}
	
	inline void split(int v, path &p, path &q) {
		path::split(get_path(v), vertex[v], p, q);
	}
	
	inline int& dparent(int v) {
		return dashed_link[v];
	}
	
	unordered_map<int, path::iterator> vertex;
	
	unordered_map<int, int> dashed_link;
};

//-------------------------------------------------
//
//
//
//-------------------------------------------------

LC_tree LCT;

int main()
{
	fstream cin("input.txt");
	ios::sync_with_stdio(false);

	return 0;
}
