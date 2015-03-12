#pragma once

#include "..\..\treap.h"
#include "..\..\mergeable_tree.h"

template <class ...Types>
class rope : public mergeable_tree<Types...> {
public:

	rope() { root = NULL; }

	void destruct() {
		delete root;
		root = NULL;
	}

	class iterator : mergeable_tree<Types...>::iterator {

		iterator(node<int, Types...> *ptr) { this->ptr = ptr; }

	public:
		friend class rope;

		iterator() { ptr = NULL; }

		std::tuple<Types...>& operator *() { return ptr->info; }

		void set_info(std::tuple<Types...> t) {
			ptr->info = t;
			im_treap<Types...>::update(ptr);
		}

		iterator left() { return iterator(node<int, Types...>::left(ptr)); }

		iterator right() { return iterator(node<int, Types...>::right(ptr)); }

		iterator parent() { return iterator(node<int, Types...>::parent(ptr)); }

		iterator root() { return iterator(node<int, Types...>::find_root(ptr)); }

		iterator& operator++ () {
			ptr = node<int, Types...>::after(ptr);
			return *this;
		}
		
		iterator& operator-- () {
			ptr = node<int, Types...>::before(ptr);
			return *this;
		}
		
		bool operator< (iterator &rhs) {
			return node<int, Types...>::pos(ptr) < node<int, Types...>::pos(rhs.ptr);
		}
		
		bool operator== (iterator &rhs) { return ptr == rhs.ptr; }
		
		bool operator!= (iterator &rhs) { return ptr != rhs.ptr; }

	private:

		node<int, Types...> *ptr;
	};
	friend class iterator;

	rope(iterator it) { root = it.ptr; }

	/*
		��������� ������ l � r, ��������� �������� � dst
		INPUT:
			l/r - ����� � ������ ������
		OUTPUT:
			dst - �������� �������������� ������
		WARNING:
			��������� ���������� l � r
	*/
	static void merge(rope<Types...> &dst,
					  rope<Types...> &l,
					  rope<Types...> &r)
	{
		im_treap<Types...>::merge(dst.root, l.root, r.root);
		l.root = r.root = NULL;
	}

	/*
		����� �������� ������ src �� ��������� it
		�� ��� �����
		INPUT:
			src   - ������
			it    - ������� � ������ src
		OUTPUT:
			l - �������� src[...it)
			r - �������� src[it...]
		WARNING:
			��������� ���������� src
	*/
	static void split(rope<Types...> &src, iterator it,
					  rope<Types...> &l,
					  rope<Types...> &r) 
	{
		int i = node<int, Types...>::pos(it.ptr);
		im_treap<Types...>::split(src.root, i, l.root, r.root);

		src.root = NULL;
	}

	void insert(Types... x, int pos) {
		im_treap<Types...>::insert(root, x..., pos);
	}

	void push_back(Types... x) {
		insert(x..., size());
	}

	iterator erase(iterator it) {
		int i = node<int, Types...>::pos(it.ptr);
		im_treap<Types...>::remove(root, i);
		return at(i);
	}

	iterator tree_root() {
		return iterator(root);
	}

	iterator at(int i) {
		return iterator(im_treap<Types...>::at(root, i));
	}

	iterator begin() { return at(0); }

	iterator rbegin() { return at(size()-1); }

	iterator end() { return iterator(); }

	int size() const { return node<int, Types...>::size_of(root); }
	
	bool empty() const { return size() == 0; }

private:

	node<int, Types...> *root;
};
