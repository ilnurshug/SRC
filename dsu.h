#pragma once
#include <vector>

/*
	Disjoint Set Union
*/
class DSU {
public:
	DSU() {}
	DSU(int n) {
		p.resize(n);
		rang.resize(n);
		for(int i = 0; i < n; i++)
			init_set(i);
	}
	
	int find_set(int i) {
		return p[i] == i ? i : p[i] = find_set(p[i]);
	}
	
	bool union_sets(int i, int j) {
		i = find_set(i), j = find_set(j);
		if (i == j) return false;
		
		if (rang[i] < rang[j]) std::swap(i,j);
		p[j] = i;
		if (rang[i] == rang[j]) rang[i]++;
		
		return true;
	}
	
private:

	void init_set(int i) {
		p[i] = i;
		rang[i] = 0;
	}

	std::vector<int> p, rang;
};