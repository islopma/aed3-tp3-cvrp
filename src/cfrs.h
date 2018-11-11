#ifndef CFRS_H_INCLUDED
#define CFRS_H_INCLUDED

#include <vector>

#include "cvrp.h"

vector<vector<Node>> clusterize(vector<Node>& nodes, int capacity);

#endif