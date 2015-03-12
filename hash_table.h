#pragma once

#include "hash_map.h"
#include "list.h"
#include <vector>

/*
	TEMPLATE ARG:
		K			- key of item
		V			- value
		Container	- коллекци€ дл€ хранени€ элементов с одтнаковым хэш-кодом
*/
template <class K, class V, template <class> class Container = list >
class hash_table : public hash_map<K, V> {
public:

	hash_table() { 
		size = 4;
		num  = 0;
		t.resize(size);
	}

	hash_table(const hash_table<K,V> &T) {
		copy(T);
	}

	hash_table & operator = (const hash_table<K,V> &T) {
		if (this == &T) return *this;

		copy(T);

		return *this;
	}

	V& operator [] (const K& x) {
		if (load_factor() >= 1.0) {
			re_hash(size * 2);
		}

		unsigned int h = hash(x);
		auto it = find(h, x);

		if (it == t[h].end()) {
			it = t[h].insert(std::pair<K,V>(x, V()));
			num++;
		}

		return (*it).second;
	}

	bool erase(const K& x) {
		unsigned int h = hash(x);
		auto it = find(h, x);

		if (it == t[h].end()) 
			return false;
		else {
			t[h].erase(it);
			num--;
			return true;
		}
	}

	bool find(const K& x) {
		unsigned int h = hash(x);
		auto it = find(h, x);

		return (it == t[h].end()) ? false : true;
	}

	int items_count() const { return num; }

	class bucket_iterator;
	friend class bucket_iterator;

	bucket_iterator begin(int i) {
		return bucket_iterator(t[i].begin());
	}

	bucket_iterator end(int i) {
		return bucket_iterator(t[i].end());
	}

	int bucket_count() const {
		return size;
	}

	class iterator;
	friend class iterator;

	iterator begin() { 
		return iterator(t.begin(), t.end(), true); 
	}

	iterator end() { 
		return iterator(t.end(), t.end(), false); 
	}

private:

	/*
		мера заполнености хэш-таблицы.
		в случае, когда это значение становитс€ больше или равно единице
		производитс€ рехэширование данных таблицы 
	*/
	inline double load_factor() {
		return (double)num / size;
	}

	/*
		изменение размера хэш-таблицы, с последующим перераспределением
		данных
		INPUT:
			n - количество €чеек в новой хэш-таблице
		WARNING:
			если n меньше текущего количества €чеек, то ничего не произойдет
	*/
	void re_hash(int n) {
		if (n <= size) return;

		int old_size = size;

		size = n;

		t.resize(n);

		for(int i = 0; i < old_size; i++)
			re_hash_bucket(i);
	}

	/*
		перераспределение данных из €чейки i по другим (в случае необходимости)
		INPUT:
			i - номер €чейки
	*/
	void re_hash_bucket(int i) {
		auto it = t[i].begin();

		while(it != t[i].end()) {
			std::pair<K,V> item = *it;
			unsigned int j = hash(item.first);

			if (j == i) 
				++it;
			else {
				it = t[i].erase(it);
				t[j].insert(item);
			}
		}
	}

	inline unsigned int hash(const K& x) {
		return hf(x) % size;
	}

	typename Container< std::pair<K,V> >::iterator find(int h, const K& x) {
		Container< std::pair<K,V> >::iterator it = t[h].begin();

		for(; it != t[h].end(); ++it) {
			if ((*it).first == x) break;
		}
		return it;
	}

	void copy(const hash_table<K,V,Container> &T) {
		t.clear();

		size = T.size;
		num  = T.num;
		t.resize(size);

		for(int i = 0; i < size; i++) {
			t[i] = T.t[i];
		}
	}

	std::vector< Container< std::pair<K,V> > > t;
	int size, num;
};

template <class K, class V, template <class> class Container = list >
class hash_table<K, V, Container>::bucket_iterator {

	typename Container< std::pair<K,V> >::iterator it;

	bucket_iterator(typename Container< std::pair<K,V> >::iterator it) {
		this->it = it;
	}

public:
	friend class hash_table;

	bucket_iterator() { it = Container< std::pair<K,V> >::iterator(); }

	const std::pair<K,V>& operator* () {
		return *it;
	}

	bucket_iterator& operator++ () {
		if (it != Container< std::pair<K,V> >::iterator()) ++it;
		return *this;
	}

	bucket_iterator& operator-- () {
		if (it != Container< std::pair<K,V> >::iterator()) --it;
		return *this;
	}

	bool operator == (const bucket_iterator &rhs) { return it == rhs.it; }

	bool operator != (const bucket_iterator &rhs) { return it != rhs.it; }
};

template <class K, class V, template <class> class Container = list >
class hash_table<K, V, Container>::iterator {

	iterator(typename std::vector< Container< std::pair<K,V> > >::iterator it, 
		typename std::vector< Container< std::pair<K,V> > >::iterator end,
		bool begin) 
	{
		this->it  = it;
		this->end = end;

		if (begin) next();
	}

public:
	friend class hash_table;

	const std::pair<K,V>& operator* () {
		return *bucket_it;
	}

	iterator& operator++ () {
		++bucket_it;

		if (bucket_it == bucket_iterator()) {
			++it;
			next();
		}

		return *this;
	}

	bool operator == (const iterator &rhs) { return it == rhs.it; }

	bool operator != (const iterator &rhs) { return it != rhs.it; }

private:

	void next() {
		for (; it != end; ++it) 
			if ((*it).begin() != (*it).end()) {
				bucket_it = bucket_iterator((*it).begin());
				break;
			}
	}
	
	bucket_iterator bucket_it;

	typename std::vector< Container< std::pair<K,V> > >::iterator it, end;
};
