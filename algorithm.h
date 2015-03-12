#pragma once
#include "dheap.h"

#define all(v) (v).begin(), (v).end()

template <class Iterator, class Function>
void _for_each(Iterator first, Iterator last, Function fn) {
	while(first != last) {
		fn(*first);
		++first;
	}
}

template <class Iterator, class T>
Iterator _find(Iterator first, Iterator last, const T& val) {
	while (first != last) {
		if (*first == val) return first;
		++first;
	}
	return last;
}

template <class Iterator, class Function>
Iterator _find_if(Iterator first, Iterator last, Function fn) {
	while (first != last) {
		if (fn(*first)) return first;
		++first;
	}
	return last;
}

template <class Iterator, class Compare, template<class,class> class Heap = dheap>
void _sort(Iterator first, Iterator last, 
		  Compare cmp) 
{
	typedef std::decay<decltype(*first)>::type T;

	Heap<T, Compare> h(cmp);

	_for_each(first, last, [&h] (T x) { h.insert(x); });

	while(!h.empty()) {
		*first++ = h.get_min();
		h.extract_min();
	}
} 