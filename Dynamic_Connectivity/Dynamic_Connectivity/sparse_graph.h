#pragma once

#include <vector>
#include "graph.h"
#include "..\..\list.h"
#include "..\..\algorithm.h"


template <class EDGE, class Container = list<EDGE>>
class sparse_graph : public GRAPH<EDGE> {
public:
	sparse_graph() { n = m = 0; }
	sparse_graph(int n, bool dir) {
		this->n = n;
		this->m = 0;
		this->dir = dir;
		g.resize(n);
	}

	EDGE edge(int v, int u) {
		return *_find_if(g[v].begin(), g[v].end(), [v, u] (EDGE e) {
			return e.v == v && e.u == u;
		});
	}

	void insert(EDGE e) {
		g[e.v].push_back(e);
		if (!directed())
			g[e.u].push_back(e);
		m++;
	}

	void remove(EDGE e) {
		g[e.v].erase(_find(g[e.v].begin(), g[e.v].end(), e));
		if (!directed()) 
			g[e.u].erase(_find(g[e.u].begin(), g[e.u].end(), e));
		m--;
	}

	bool path(int v, int u) {
		list<int> q; q.push_back(v);
		std::vector<char> used(n, false); used[v] = true;

		while(!q.empty()) {
			int from = q.front(); q.pop_front();

			_for_each(g[from].begin(), g[from].end(), [&] (EDGE e) {
				int to = (e.v == from) ? e.u : e.v;
				if (!used[to]) {
					used[to] = true;
					q.push_back(to);
				}
			});
		}

		return used[u];
	}

	class iterator {
	private:
		typename Container::iterator adj;

	public:
		friend class sparse_graph;

		iterator (typename Container::iterator it) { adj = it; }

		EDGE operator * () { return *adj; }

		iterator& operator++ () {
			++adj;
			return *this;
		}

		bool operator == (const iterator& rhs) { return adj == rhs.adj; }

		bool operator != (const iterator& rhs) { return adj != rhs.adj; }
	};

	friend class iterator;

	iterator begin(int i) {
		return iterator(g[i].begin());
	}

	iterator end(int i) {
		return iterator(g[i].end());
	}

private:

	std::vector< Container > g;
};
