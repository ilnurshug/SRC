#pragma once

template <class T>
class hash_func {
public:
	virtual unsigned long long operator () (const T& obj) = 0;
};

template<>
class hash_func<int> {
public:
	unsigned long long operator () (const int& obj) {
		return (obj < 0 ? -obj : obj);
	}
};

template <class K, class V>
class hash_map {
public:
	virtual V& operator [] (const K& x) = 0;

	virtual bool erase(const K& x) = 0;

	virtual bool find(const K& x) = 0;

	class iterator;
	friend class iterator;

	iterator begin();

	iterator end();

protected:

	hash_func<K> hf;
};