// Dynamic_MSF.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <vld.h>
#include <iostream>
#include "..\..\algorithm.h"
#include "..\..\avl_tree.h"
#include "..\..\Kruskal_MST.h"
#include "..\..\Dynamic_Connectivity\Dynamic_Connectivity\graph.h"
#include "..\..\Dynamic_Connectivity\Dynamic_Connectivity\sparse_graph.h"
#include "..\..\Dynamic_Connectivity\Dynamic_Connectivity\dynamic_graph.h"
#include <functional>

template <class EDGE, template<class...> class GRAPH>
class Decremental_MSF {
public:
	Decremental_MSF() {}

	Decremental_MSF(GRAPH<EDGE> &G, 
					std::function<void(GRAPH<EDGE> &, 
									   std::vector<EDGE> &, 
									   std::vector<EDGE> &)
					> mst_algo) 
	{
		D.construct(G.V());
		
		std::vector<EDGE> mst, non_mst;
		mst_algo(G, mst, non_mst);

		batch_insert(mst);
		batch_insert(non_mst);
	}

	inline void remove(EDGE e) {
		auto re = D.remove(e);
		std::cout << re << '\n';
	}

	inline bool connected(int v, int u) {
		return D.path(v, u);
	}

	bool msf_edge(int v, int u) {
		return D.msf_edge(v, u);
	}

private:

	inline void batch_insert(std::vector<EDGE> &lst) {
		_for_each(lst.begin(), lst.end(), [this] (EDGE& e) {
			this->D.insert(e);
		});
	}

	dynamic_graph< sparse_graph<edge, avl_tree<edge>> > D;
};

int main()
{
	sparse_graph<edge> g(7, false);
	g.insert(edge(1, 2, 1));
	g.insert(edge(2, 3, 2));
	g.insert(edge(4, 2, 3));
	g.insert(edge(5, 4, 4));
	g.insert(edge(4, 6, 5));

	g.insert(edge(1, 3, 6));
	g.insert(edge(1, 5, 7));
	g.insert(edge(1, 6, 8));

	g.insert(edge(3, 6, 9));

	Decremental_MSF<edge, sparse_graph> d_msf(g, Kruskal_MST<edge, sparse_graph>);
	d_msf.remove(edge(1,2,1));
	d_msf.remove(edge(4,6,5));
	d_msf.remove(edge(1,6,8));
	d_msf.remove(edge(3,6,9));


	return 0;
}

