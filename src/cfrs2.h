#ifndef CFRS2_H_INCLUDED
#define CFRS2_H_INCLUDED

#include <vector>

#include "cvrp.h"

std::vector<std::vector<Node>> clusterize_kruskal(std::vector<Node> &nodes, double capacity);

std::vector<Node> farthest_insertion(std::vector<Node> &nodes);

#endif