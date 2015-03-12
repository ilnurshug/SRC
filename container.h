#pragma once

template <class T>
class _container {
public:

	class iterator;
	friend class iterator;

	iterator begin();

	iterator end();

	iterator insert(T x);

	iterator erase(iterator it);
};