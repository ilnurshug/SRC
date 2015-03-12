#pragma once

template <class EDGE>
class GRAPH {
public:
	GRAPH() { n = m = 0; }

	GRAPH(int n, bool dir);

	int V() const { return n; }

	int E() const { return m; }

	bool directed() const { return dir; }

	virtual EDGE edge(int v, int u) = 0;

	virtual void insert(EDGE e) = 0;

	virtual void remove(EDGE e) = 0;

	virtual bool path(int v, int u) = 0;

protected:

	int n, m;
	bool dir;
};
