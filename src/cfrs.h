#ifndef CFRS_H_INCLUDED
#define CFRS_H_INCLUDED

#include <vector>

#include "cvrp.h"

std::vector<std::vector<Node>> clusterize(std::vector<Node> &nodes, int capacity);

void route(std::vector<Node> &nodes);

double get_cost(std::vector<Node> &nodes);

#endif