#pragma once
#include "algorithm.h"
#include "dsu.h"
#include <vector>

template <class EDGE, template<class...> class GRAPH>
void edge_list(GRAPH<EDGE> &G, std::vector<EDGE> &el) {
	el.resize(G.E());
	
	for(int v = 0, E = 0; v < G.V(); v++) {
		GRAPH<EDGE>::iterator it = G.begin(v);
		for(; it != G.end(v); ++it) {
			EDGE e = *it;
			if (e.v == v) el[E++] = e;
		}
	}
}

template <class EDGE, template <class...> class GRAPH >
void Kruskal_MST(GRAPH<EDGE> &G, 
				 std::vector<EDGE> &mst, 
				 std::vector<EDGE> &non_mst) 
{
	std::vector<EDGE> el;
	edge_list<EDGE, GRAPH>(G, el);

	_sort(all(el), [] (EDGE lhs, EDGE rhs) {
		return lhs.weight < rhs.weight;
	});
		
	DSU dsu(G.V());
	for(auto e : el) {
		if (dsu.union_sets(e.v, e.u)) 
			mst.push_back(e);
		else
			non_mst.push_back(e);
	}
}	