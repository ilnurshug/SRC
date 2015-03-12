#pragma once
#include "priority_queue.h"
#include <vector>

template <class T, class Compare>
class dheap : public priority_queue<T, Compare> {
private:

	int d, sz;
	Compare cmp;
	std::vector<T> h;

	int p(int i) { return (i - 2) / d + 1; }
	
	int l(int i) { 
		int k = (i - 1) * d + 2;
		return k < sz ? k : 0;
	}
	
	int r(int i) {
		int k = l(i);
		return k > 0 ? min(k + d - 1, sz - 1) : 0;
	}
	
public:
	dheap() {}

	dheap(Compare cmp, int d = 2) {
		this->d = d;
		this->cmp = cmp;
		sz = 1;
		h.resize(4); 
	}
	
	void insert(T x) {
		if(sz == h.size()) h.resize(h.size() * 2); 

		h[sz] = x;
		for(int i = sz; i != 1 && cmp(h[i], h[p(i)]); i = p(i))
			std::swap(h[p(i)], h[i]);
		sz++; 
	}
	
	void extract_min() {
		if(sz == 1) return;
		h[1] = h[--sz];

		int i = 1, lft = l(i), rht = r(i);
		while(rht != 0 && lft != 0) {
			int min = i;
			for(int j = lft; j <= rht; j++)
				if (cmp(h[j],h[min])) min = j;
				
			if(min == i) break;

			std::swap(h[i], h[min]);
			
			i = min;
			lft = l(i), rht = r(i);
		}
	}
	
	T get_min() { return h[1]; }
	
	bool empty() { return sz==1; }
};