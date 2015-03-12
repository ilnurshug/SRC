#pragma once

#include "sparse_graph.h"
#include "..\..\hash_table.h"

#include "..\..\Euler_Tour_tree\Euler_Tour_tree\et_tree.h"


class edge_info {
public:
	edge_info(bool is_level_i_edge = false, 
			  int level_i_adj_edge = 0, 
			  bool exists_level_i_edge = false, 
			  int cnt_level_i_adj_edge = 0) 
	{
		IS_EDGE() = is_level_i_edge;
		EX_EDGE() = exists_level_i_edge;

		ADJ() = level_i_adj_edge;
		CNT_ADJ() = cnt_level_i_adj_edge;
	}

	bool& IS_EDGE() { return is_level_i_edge; }

	bool& EX_EDGE() { return exists_level_i_edge; }

	int& ADJ() { return level_i_adj_edge; }

	int& CNT_ADJ() { return cnt_level_i_adj_edge; }

private:
	
	bool is_level_i_edge;
	bool exists_level_i_edge;

	int level_i_adj_edge;
	int cnt_level_i_adj_edge;
};

template <>
class tree_update<edge, edge_info> {
public:
	static std::tuple<edge, edge_info> zero() { 
		return std::make_tuple(edge(), edge_info());
	}

	static std::tuple<edge, edge_info> append(std::tuple<edge, edge_info> &rt, 
										 std::tuple<edge, edge_info> &l, 
									     std::tuple<edge, edge_info> &r)
	{
		return std::make_tuple(std::get<0>(rt), 
							   append(std::get<1>(rt), std::get<1>(l), std::get<1>(r))
							   );
	}

private:

	static edge_info append(edge_info rt, edge_info l, edge_info r) { 
		edge_info tmp;

		tmp.IS_EDGE() = rt.IS_EDGE();
		tmp.EX_EDGE() = rt.IS_EDGE() || l.EX_EDGE() || r.EX_EDGE();

		tmp.ADJ() = rt.ADJ();
		tmp.CNT_ADJ() = rt.ADJ() + l.CNT_ADJ() + r.CNT_ADJ();

		return tmp;
	}
};


/*
	sparse_graph<edge, container<edge>>,
	edge - from ET_tree
*/
/*
	����� ������������ ����������������� ���������� ���� 
	��� ������� ����� � ������
	��������� ��������� ������� �������, �������� � ��������� ��
	��������������� ����� O(log n)^2.
*/
template <class graph>
class dynamic_graph {
public:

	typedef ET_tree< edge_info > ETT;

	dynamic_graph() { n = m = 0; }
	dynamic_graph(int n) {
		construct(n);
	}

	void construct(int n) {
		this->n = n;
		this->m = 0;

		L = (int)floor(log(n)/log(2.0));
		G.resize(L+1);
		F.resize(L+1);

		for(int i = 0; i <= L; i++) 
			init(i);
	}

	/* ���������� ������ � ����� */
	int V() const { return n; }

	/* ���������� ����� � ����� */
	int E() const { return m; }

	/*
		������������� ���� ����� ��������� v � u
		INPUT:
			v/u - vertices
		OUTPUT:
			0/1 - (��)����������
	*/	
	inline bool path(int v, int u) {
		return F[0].path(v, u);
	}

	/*
		�������� � ���� ����� e = (v, u, w)
		REQUIREMENTS:
			� ����� �� ������ ���� ����� (v, u)
	*/
	inline void insert(int v, int u, double w = 1) {
		insert(edge(v, u, w));
	}

	/*
		�������� � ���� ����� e = (v, u, w)
		REQUIREMENTS:
			� ����� �� ������ ���� ����� (v, u)
	*/
	void insert(edge e) {
		int v = e.v, u = e.u;
		l(v, u) = l(u, v) = 0;

		if (F[0].path(v, u)) insert_non_tree_edge(e, 0);
		else				 insert_tree_edge(e, 0);
		m++;
	}

	/*
		������� �� ����� ����� e = (v, u, w)
		REQUIREMENTS:
			� ����� ������ ���� ����� (v, u)
	*/
	inline void remove(int v, int u, double w = 1) {
		remove(edge(v, u, w));
	}

	/*
		������� �� ����� ����� e = (v, u, w),
		���������� replacement edge, ���� ��� ���� �������
		REQUIREMENTS:
			� ����� ������ ���� ����� (v, u)
	*/
	edge remove(edge e) {
		edge rep_edge;

		int v = e.v, u = e.u;
		int level = l(v, u);

		l(v, u) = l(u, v) = 0;

		if (tree_edge(v, u, level)) {
			set_tree_edge(v, u, level, false);
			
			for(int i = level; i >= 0; i--) 
				remove_tree_edge(e, i);
			
			bool replacement = false;
			edge f;
			replace(v, u, level, replacement, f);
			
			if (replacement) {
				level = l(f.v, f.u);

				for(int i = level; i >= 0; i--) 
					F[i].link(f.v, f.u, f.weight);
				
				v = f.v;
				u = f.u;

				set_tree_edge(v, u, level, true);

				set_non_tree_edge(v, level, -1);
				set_non_tree_edge(u, level, -1);

				rep_edge = f;
			}
		}
		else {
			set_non_tree_edge(v, level, -1);
			set_non_tree_edge(u, level, -1);

			for(int i = level; i >= 0; i--)
				remove_non_tree_edge(e, i);
		}
		m--;

		return rep_edge;
	}

	inline bool msf_edge(int v, int u) {
		return tree_edge(v, u, 0);
	}

protected:

	/*
		���������� ������ � ��������� ������ T
	*/
	inline int vert_cnt(ETT::_rope &T) {
		return (T.size() + 2) / 3;
	}

	/*
		������� ����� ������ level � ������ (Euler Tour), 
		�� ������� ��������� �������� v,
		����� ������ level ������������ �� ��������� �������
	*/
	void walk_tree_edge(ETT::_rope::iterator v, int level) {
		if (v == ETT::_rope::iterator()) return;

		if (get_edge_info(v).IS_EDGE()) {
			int a = get_edge(v).v, b = get_edge(v).u;

			set_tree_edge(a, b, level, false);
			l(a, b) = l(b, a) = level + 1;
			insert_tree_edge(get_edge(v), level + 1);
		}

		if (get_edge_info(v.left()).EX_EDGE()) 
			walk_tree_edge(v.left(), level);

		if (get_edge_info(v.right()).EX_EDGE()) 
			walk_tree_edge(v.right(), level);
	}

	/*
		���������: ����� �� ����� (v, u) ��������� ������� � ������� root1 � root2
		�� ������ level
	*/
	bool reconnect(int root1, int root2, int v, int u, int level) {
		int r1 = F[level].find_root(v),
			r2 = F[level].find_root(u);

		return (r1 == root1 && r2 == root2) || (r1 == root2 && r2 == root1);
	}

	/*
		�������� ������� non-tree edges ������ level, ������ �� ������� ��������� v,
		� ������� ��������� ������� � ������� Tv_root, Tu_root
		INPUT:
			v				 - ��������� � ��������� ������ ������
			Tv_root, Tu_root - ����� ��������
			level			 - �������
		OUTPUT:
			find     - true if replacement edge was found, false otherwise
			rep_edge - replacement edge
	*/
	void walk_adj_edge(ETT::_rope::iterator v, 
					   int Tv_root, int Tu_root, 
					   int level, 
					   bool &find, edge &rep_edge) 
	{
		if (v == ETT::_rope::iterator() || find) return;
		
		if (get_edge_info(v).ADJ() > 0) {
			int a = get_edge(v).v;					// (*v).v == (*v).u => edge(v,v)

			graph::iterator it = G[level].begin(a);
			for(; it != G[level].end(a); ++it) {
				int b = (*it).v == a ? (*it).u : (*it).v;
				if (l(a, b) != level) continue;

				if (reconnect(Tv_root, Tu_root, a, b, level)) {
					find = true;
					rep_edge = *it;
					break;
				}
				else {
					set_non_tree_edge(a, level, -1);
					set_non_tree_edge(b, level, -1);

					l(a, b) = l(b, a) = level + 1;
					insert_non_tree_edge(*it, level + 1);
				}
			}
		}
		
		if (get_edge_info(v.left()).CNT_ADJ() > 0) 
			walk_adj_edge(v.left(), Tv_root, Tu_root, level, find, rep_edge);

		if (get_edge_info(v.right()).CNT_ADJ() > 0) 
			walk_adj_edge(v.right(), Tv_root, Tu_root, level, find, rep_edge);
	}

	/*
		����� ������ ����� (v, u)
		INPUT:
			v,u - vertices
			i	- level
		OUTPUT:
			find     - true if replacement edge was found, false otherwise
			rep_edge - replacement edge
	*/
	void replace(int v, int u, int i, bool &find, edge &rep_edge) {
		ETT::_rope Tv( F[i].find(v,v).root() ), 
				   Tu( F[i].find(u,u).root() );
	
		if (vert_cnt(Tv) > vert_cnt(Tu)) std::swap(Tv, Tu);

		walk_tree_edge(Tv.tree_root(), i);

		walk_adj_edge(Tv.tree_root(), 
					  F[i].find_root(v), 
					  F[i].find_root(u), 
					  i, find, rep_edge
					  );

		if (!find && i > 0) 
			replace(v, u, i-1, find, rep_edge);
	}

	/*
		������� ����� (v, u)
	*/
	int& l(int v, int u) {
		return _l[v][u];
	}

	/*
		�������� �� ����� (v, u) ������ ��������� ���� �� ������ level
	*/
	bool tree_edge(int v, int u, int level) {
		ETT::_rope::iterator it = F[level].find(v, u);
		return it != ETT::_rope::iterator();
	}

	void insert_non_tree_edge(edge e, int level) {
		G[level].insert(e);

		set_non_tree_edge(e.v, level, 1);
		set_non_tree_edge(e.u, level, 1);
	}

	inline void remove_non_tree_edge(edge e, int level) {
		G[level].remove(e);
	}

	/*
		���������/��������� ������� ���������� ������� non-tree ����� 
		������ level ������� v
		INPUT:
			v     - �������
			level - �������
			f     - +1 ��� -1 � ����������� �� ������������ ��������
	*/
	void set_non_tree_edge(int v, int level, int f) {      // f = +- 1
		ETT::_rope::iterator it = F[level].find(v, v);

		edge_info it_info = get_edge_info(it),
			l_info = get_edge_info(it.left()),
			r_info = get_edge_info(it.right());

		it_info.ADJ() += f;
		it_info.CNT_ADJ() += f;

		it.set_info( std::make_tuple(get_edge(it), it_info) );
	}

	/*
		�������� ����� (v, u) � �������� ��� �� ������ level
	*/
	void insert_tree_edge(edge e, int level) {
		F[level].link(e.v, e.u, e.weight);
		G[level].insert(e);

		set_tree_edge(e.v, e.u, level, true);
	}

	/*
		������� ����� ��������� ���� �� ����� �� ������ level
	*/
	inline void remove_tree_edge(edge e, int level) {
		F[level].cut(e.v, e.u);
		G[level].remove(e);
	}

	/*
		�������� ���������� �� ������ �� ������������� ����� (v, u)
		INPUT:
			(v, u) - �����
			level  - �������
			f      - true - ����� ������������ �� ������� level
					 false - ����� ������������ � ������ level
	*/
	inline void set_tree_edge(int v, int u, int level, bool f) {
		set_tree_dir_edge(v, u, level, f);
		set_tree_dir_edge(u, v, level, f);
	}

	/*
		�������� ���������� �� ������ ������������� ����� (v, u)
		INPUT:
			(v, u) - �����
			level  - �������
			f      - true - ����� ������������ �� ������� level
					 false - ����� ������������ � ������ level
	*/
	void set_tree_dir_edge(int v, int u, int level, bool f) {
		ETT::_rope::iterator it = F[level].find(v, u);

		edge_info it_info = get_edge_info(it),
			l_info = get_edge_info(it.left()),
			r_info = get_edge_info(it.right());

		it_info.IS_EDGE() = f;
		it_info.EX_EDGE() =  f || l_info.EX_EDGE() || r_info.EX_EDGE();

		it.set_info( std::make_tuple(get_edge(it), it_info) );
	}

	void init(int level) {
		G[level] = graph(n, false);

		for(int i = 0; i < n; i++)
			F[level].make_tree(i);
	}

	inline edge get_edge(ETT::_rope::iterator& it) {
		return std::get<0>(*it);
	}

	inline edge_info get_edge_info(ETT::_rope::iterator& it) {
		return it != ETT::_rope::iterator() ? std::get<1>(*it) : edge_info();
	}

	int L;
	int n, m;

	//std::vector< std::vector<int> > _l;
	hash_table< int, hash_table<int, int> > _l;

	std::vector< ETT > F;

	std::vector< graph > G;

};
