#ifndef UNION_FIND_H_INCLUDED
#define UNION_FIND_H_INCLUDED

#include <vector>

class UnionFind
{

private:

    int n;
    std::vector<int> _parent;
    std::vector<int> _rank;

public:

    UnionFind(int n);

    int join(int x, int y);

    int group(int x);


};


#endif