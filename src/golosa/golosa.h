
#ifndef CVRP_GOLOSA_H
#define CVRP_GOLOSA_H

#include "cvrp.h"
#include <vector>

using namespace std;


    bool areThereNodesLeft(vector<bool> &usedNodes);

    vector<vector<double>> getDistances(vector<Node> &nodes);

    int closestNeighbor(Node &node, int maxDemand, vector<vector<double>>& distances, vector<bool> usedNodes, vector<Node> &nodes, int candidato);

    void getUsedNodes(vector<bool> &nodes, int size);


    vector<Route> algoGoloso(vector<Node> &nodes,int capacity, Cvrp &cvrp);

    int areThereNodesWithSmallerDemand(Node &node , int maxDemand, vector<bool> _usedNodes, vector<Node> &_nodes);

    void addNodeToRuta(Route &route, Node &node, const bool &isLast, vector<vector<double>> costs);







#endif //CVRP_GOLOSA_H
