#include "cfrs2.h"

#include <utility>
#include <tuple>
#include <cmath>
#include <algorithm>

#include "union_find.h"

#include <cassert>

struct edge
{
    int x;
    int y;
    double distance;
};

bool operator<(const edge &e, const edge &f)
{
    return make_tuple(e.distance, e.x, e.y) < make_tuple(f.distance, f.x, f.y);
}

double get_distance(const Node &a, const Node &b)
{
    return hypot(a.x - b.x, a.y - b.y);
}

double get_distance_to_segment(const Node &a, const Node &b, const Node &p)
{
    double ab = get_distance(a, b);
    double pa = get_distance(p, a);
    double pb = get_distance(p, b);
    if (pa * pa > pb * pb + ab * ab)
        return pb;
    if (pb * pb > pa * pa + ab * ab)
        return pa;

    double areax2 = abs((a.x - p.x) * (b.y - p.y) - (a.y - p.y) * (b.x - p.x));

    return areax2 / ab;

    // if(p2.sub(p1).dot(q.sub(p1)) < EPS) return q.sub(p1).abs();
    // if(p1.sub(p2).dot(q.sub(p2)) < EPS) return q.sub(p2).abs();
    // return disLP(p1, p2, q);
}

vector<edge> get_sorted_edges(vector<Node> &nodes)
{

    vector<edge> res;
    int n = nodes.size();

    for (int i = 1; i < n; ++i)
    {
        for (int j = 1; j < i; ++j)
        {
            double distance = get_distance(nodes[i], nodes[j]);
            res.push_back({i, j, distance});
        }
    }
    sort(begin(res), end(res));
    return res;
}

vector<vector<Node>> make_groups_from_uf(vector<Node> &nodes, UnionFind &uf)
{
    int n = nodes.size();
    vector<vector<int>> with_empty_lists(n);

    for (int i = 1; i < n; ++i)
    {
        with_empty_lists[uf.group(i)].push_back(i);
    }

    vector<vector<Node>> res;
    for (auto &list : with_empty_lists)
    {
        if (!list.empty())
        {
            vector<Node> cluster = {nodes[0]};
            for (auto i : list)
            {
                cluster.push_back(nodes[i]);
            }
            res.push_back(cluster);
        }
    }

    return res;
}

vector<vector<Node>> clusterize(vector<Node> &nodes, double capacity)
{

    int n = nodes.size();

    auto uf = UnionFind(n);

    auto sorted_edges = get_sorted_edges(nodes);

    vector<double> demand_sum(n, 0);
    for (int i = 1; i < n; ++i)
    {
        demand_sum[i] = nodes[i].demand;
    }

    for (auto &e : sorted_edges)
    {
        int gx = uf.group(e.x);
        int gy = uf.group(e.y);
        if (gx != gy)
        {
            double total_demand = demand_sum[gx] + demand_sum[gy];
            if (total_demand <= capacity)
            {
                demand_sum[uf.join(e.x, e.y)] = total_demand;
            }
        }
    }

    return make_groups_from_uf(nodes, uf);
}

pair<int, int> find_farthest_nodes(vector<Node> &nodes)
{

    double max_dist = 0;
    double a = -1;
    double b = -1;

    int n = nodes.size();
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            double distance = get_distance(nodes[i], nodes[j]);
            if (distance > max_dist)
            {
                max_dist = distance;
                a = i;
                b = j;
            }
        }
    }

    return {a, b};
}

int find_farthest_node(vector<Node> &nodes, vector<int> &_next, int first, vector<bool> &visited)
{
    int n = nodes.size();

    int fartest = -1;
    double max_dist = 0;

    for (int i = 0; i < n; ++i)
    {
        if (!visited[i])
        {
            double min_dist_to_i = get_distance(nodes[i], nodes[first]);
            for (int c = _next[first]; c != first; c = _next[c])
            {
                min_dist_to_i = min(min_dist_to_i, get_distance(nodes[i], nodes[c]));
            }

            if (min_dist_to_i > max_dist)
            {
                fartest = i;
                max_dist = min_dist_to_i;
            }
        }
    }

    return fartest;
}

int find_closest_edge(vector<Node> &nodes, vector<int> &_next, int first, int i)
{
    int best_a = first;
    double min_dist = get_distance_to_segment(nodes[first], nodes[_next[first]], nodes[i]);

    for (int a = _next[first]; a != first; a = _next[a])
    {
        double dist = get_distance_to_segment(nodes[a], nodes[_next[a]], nodes[i]);
        if (dist < min_dist)
        {
            best_a = a;
            min_dist = dist;
        }
    }

    return best_a;
}

vector<Node> farthest_insertion(vector<Node> &nodes)
{
    int n = nodes.size();
    if (n < 3)
        return nodes;

    vector<int> _next(n);
    iota(begin(_next), end(_next), 0);

    vector<bool> in_cycle(n, false);

    auto p = find_farthest_nodes(nodes);
    _next[p.first] = p.second;
    _next[p.second] = p.first;
    in_cycle[p.first] = in_cycle[p.second] = true;

    int third = find_farthest_node(nodes, _next, p.first, in_cycle);
    _next[p.second] = third;
    _next[third] = p.first;
    in_cycle[third] = true;

    for (int i = 0; i < n - 3; ++i)
    {
        int fartest = find_farthest_node(nodes, _next, p.first, in_cycle);
        int pos = find_closest_edge(nodes, _next, p.first, fartest);
        _next[fartest] = _next[pos];
        _next[pos] = fartest;
    }

    vector<Node> res = {nodes[0]};
    for (int c = _next[0]; c != 0; c = _next[c])
    {
        res.push_back(nodes[c]);
    }

    return res;
}