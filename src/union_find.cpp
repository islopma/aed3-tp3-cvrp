#include "union_find.h"

#include <algorithm>

UnionFind::UnionFind(int n)
{
    this->n = n;
    this->_parent = std::vector<int>(n);
    iota(begin(this->_parent), end(this->_parent), 0);
    this->_rank = std::vector<int>(n, 0);
}

int UnionFind::group(int x)
{

    if (_parent[_parent[x]] != _parent[x])
    {
        _parent[x] = group(_parent[x]);
    }
    return _parent[x];
}

int UnionFind::join(int x, int y)
{
    if (group(x) != group(y))
    {

        int px = _parent[x];
        int py = _parent[y];

        if (_rank[px] == _rank[py])
            _rank[px]++;

        if (_rank[px] < _rank[py])
        {
            px = py;
            py = _parent[x];
        }

        _parent[py] = px;
    }
    return group(x);
}