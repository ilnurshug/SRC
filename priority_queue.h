#pragma once

template <class T, class Compare> 
class priority_queue {
public:
	priority_queue(){}
	priority_queue(Compare cmp);
	
	virtual void insert(T x) = 0;
	
	virtual void extract_min() = 0;
	
	virtual T get_min() = 0;
	
	virtual bool empty() = 0;
};