#pragma once

#include <tuple>

template <class... Types>
class mergeable_tree {
public:
	
	mergeable_tree() {}

	class iterator {
	public:
		friend class mergeable_tree;

		iterator() { }

		const std::tuple<Types...>& operator* ();

		void set_info(std::tuple<Types...> t);
		
		iterator& operator++ ();
		
		iterator& operator-- ();
		
		bool operator< (iterator &rhs);
		
		bool operator== (iterator &rhs);
		
		bool operator!= (iterator &rhs);
		
		iterator root();

		iterator parent();
		
		iterator left();
		
		iterator right();
	};
	friend class iterator;

	mergeable_tree(iterator root);
	
	static void split(mergeable_tree<Types...> &src, iterator i,
					  mergeable_tree<Types...> &l,
					  mergeable_tree<Types...> &r);
					  
	static void merge(mergeable_tree<Types...> &dst,
					  mergeable_tree<Types...> &l,
					  mergeable_tree<Types...> &r);
	
	void insert(Types... x, int i);
	
	iterator erase(iterator it);

	iterator tree_root();
	
	iterator at(int i);
	
	int size() const;
	
	bool empty() const;
};