#pragma once

#include <iostream>
#include "..\..\list.h"
#include "..\..\hash_table.h"
#include "rope.h"

#include <unordered_map>

class edge {
public:
	int v, u;
	double weight;

	edge(int v = 0, int u = 0, double w = 1) : v(v), u(u), weight(w) {}

	friend std::ostream& operator<< (std::ostream& stream, const edge &r) {
		stream << '(' << r.v << ',' << r.u << ',' << r.weight << ')';
		return stream;
	}

	bool operator < (const edge& rhs) const {
		return less(*this, rhs);
	}

	bool operator > (const edge& rhs) const {
		return !equal(*this, rhs) && !less(*this, rhs);
	}

	/*
		компаратор для НЕ ориентированного ребра
	*/
	bool operator== (const edge& rhs) {
		return equal(*this, rhs);
	}

private:

	bool equal (const edge& lhs, const edge& rhs) const {
		return min(lhs.v, lhs.u) == min(rhs.v, rhs.u) && 
			   max(lhs.v, lhs.u) == max(rhs.v, rhs.u) && 
			   lhs.weight == rhs.weight;
	}

	bool less(const edge& lhs, const edge& rhs) const {
		if (lhs.weight < rhs.weight) return true;
		else if (lhs.weight == rhs.weight) {
			if (min(lhs.v, lhs.u) < min(rhs.v, rhs.u)) return true;
			else if (min(lhs.v, lhs.u) == min(rhs.v, rhs.u)) 
				return max(lhs.v, lhs.u) < max(rhs.v, rhs.u); 
			else return false;
		}
		else return false;
	}
};

template <class I, 
		  template <class, class, template<class> class...> class Map = hash_table,
		  template <class...> class Rope = rope
>
class ET_tree {
public:

	typedef Rope<edge, I> _rope;

	ET_tree() { }

	~ET_tree() {
		Map<int, _rope >::iterator it = et_trees.begin();
		for(; it != et_trees.end(); ++it) {
			_rope r = (*it).second; 
			r.destruct();
		}
	}
	/*
		создает новое дерево, состоящее из одного узла с ключом v
		INPUT:
			v - key of new node
		REQUIREMENTS:
			в лесу не должно быть узла с ключом v
	*/
	void make_tree(int v) {
		_rope et;
		et.push_back(edge(v, v), I());

		et_trees[v] = et;
		edge_list[v][v] = et.begin();
	}

	/*
		соеденить ребром вершины v и u,
		после чего v становится корнем образовавшегося дерева
		INPUT:
			v\u - ключи вершин
			w   - вес ребра
		REQUIREMENTS:
			v и u находятся в разных деревьях
	*/
	void link(int v, int u, double w = 1) {
		make_root(v);
		make_root(u);

		_rope& r_v = et_trees[v];
		_rope& r_u = et_trees[u];

		edge forward = edge(v, u, w), backward = edge(u, v, w);

		r_v.push_back(forward, I());
		r_u.push_back(backward, I());

		edge_list[v][u] = r_v.rbegin();
		edge_list[u][v] = r_u.rbegin();

		_rope r;
		_rope::merge(r, r_v, r_u);
		et_trees.erase(u);
		et_trees[v] = r;
	}

	/*
		удаляет ребро (v,u)
		INPUT:
			v/u - ключи вершин (концы ребер)
		REQUIREMENTS:
			ребро (v,u) должно присутствовать в лесу
	*/
	void cut(int v, int u) {
		_rope::iterator forward, backward;
		forward = find(v, u);
		backward = find(u, v);

		erase(v, u);
		erase(u, v);

		if (backward < forward) std::swap(forward, backward);

		_rope T(forward.root()), T2;
		_rope L1, L2, L3;

		_rope::split(T, forward, L1, T2);
		T2.erase(T2.begin());
		_rope::split(T2, backward, L2, L3);
		L3.erase(L3.begin());
		_rope::merge(T, L1, L3);

		int T1_id = find_root(T), T2_id = find_root(L2);
		et_trees[T2_id] = L2;
		et_trees[T1_id] = T;
	}

	/*
		возвращает корень дерева, в котором находится узел с ключом v
		INPUT:
			v - key of the node
		OUTPUT:
			key of the root
	*/
	int find_root(int v) {
		_rope r(find(v,v).root());
		return find_root(r);
	}

	/*
		переподвешивает дерево содержащее узел v
		таким образом, что v теперь корень дерева
		INPUT:
			v - key of the node
	*/
	void make_root(int v) {
		_rope::iterator it = find(v, v);
		int r_id;

		_rope r(it.root()), left, right;
		r_id = find_root(r);

		_rope::split(r, ++it, left, right);

		et_trees.erase(r_id);

		_rope::merge(r, right, left);
		et_trees[v] = r;
	}

	/*
		существует ли путь между вершинами v и u
		(находятся ли они в одном дереве)
		INPUT:
			v\u - keys
		OUTPUT:
			0 - there is no path between v and u
			1 - otherwise
	*/
	bool path(int v, int u) {
		return find_root(v) == find_root(u);
	}
	
	inline typename _rope::iterator find(int v, int u) {
		return edge_list[v][u];
	}

	inline void erase(int v, int u) {
		edge_list[v].erase(u);
	}

//private:

	int find_root(_rope &r) {
		return std::get<0>(*(r.begin())).v;
	}

	Map< int, _rope > et_trees;

	Map<int, Map< int, typename _rope::iterator > > edge_list;
};

