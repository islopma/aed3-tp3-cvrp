#ifndef CFRS2_H_INCLUDED
#define CFRS2_H_INCLUDED

#include <vector>

#include "cvrp.h"

vector<vector<Node>> clusterize(vector<Node> &nodes, double capacity);

vector<Node> farthest_insertion(vector<Node> &nodes);

#endif