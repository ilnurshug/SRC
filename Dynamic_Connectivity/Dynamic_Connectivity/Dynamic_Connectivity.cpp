// Dynamic_Connectivity.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <vld.h>
#include <iostream>

#include "sparse_graph.h"
#include "dynamic_graph.h"
#include "..\..\avl_tree.h"


int main()
{
	dynamic_graph<sparse_graph<edge, avl_tree<edge>>> g(10);
	//sparse_graph<edge> g(10, false);

	g.insert(edge(1, 2, 1));
	g.insert(edge(2, 3, 2));
	g.insert(edge(4, 2, 3));
	g.insert(edge(5, 4, 4));
	g.insert(edge(4, 6, 5));

	g.insert(edge(1, 3, 6));
	g.insert(edge(1, 5, 7));
	g.insert(edge(1, 6, 8));

	g.insert(edge(3, 6, 9));

	g.remove(edge(1,2,1));
	g.remove(edge(4,6,5));
	g.remove(edge(1,6,8));
	g.remove(edge(3,6,9));

	/*D.insert(1, 2);
	D.insert(1, 3);
	D.insert(1, 4);
	D.insert(2, 5);
	D.insert(2, 6);
	D.insert(4, 8);
	D.insert(8, 7);
	D.insert(7, 9);

	D.insert(2, 3);
	D.insert(5, 6);
	D.insert(3, 6);
	D.insert(4, 6);
	D.insert(8, 9);

	std::cout << D.path(6, 4) << '\n';
	D.remove(6, 4);
	std::cout << D.path(6, 4) << '\n';

	std::cout << D.path(2, 5) << '\n';
	D.remove(5, 2);
	std::cout << D.path(2, 5) << '\n';

	std::cout << D.path(4, 8) << '\n';
	D.remove(4, 8);
	std::cout << D.path(4, 8) << '\n';

	std::cout << D.path(1, 2) << '\n';
	D.remove(2, 1);
	std::cout << D.path(2, 1) << '\n';

	std::cout << D.path(2, 3) << '\n';
	D.remove(3, 2);
	std::cout << D.path(3, 2) << '\n';*/

	return 0;
}

