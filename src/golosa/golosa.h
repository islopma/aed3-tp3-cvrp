
#ifndef CVRP_GOLOSA_H
#define CVRP_GOLOSA_H

#include "cvrp.h"
#include <vector>

using namespace std;

class Golosa{

private:

    Cvrp _cvrp;
    vector<Node> &_nodes = _cvrp.getNodes();
    bool areThereNodesLeft();
    int nodesSize = _cvrp.getNodes().size();
    vector<bool> _usedNodes = vector<bool>(nodesSize, false);        //si el nodo esta usado aparece como true en su indice (nodeID-1)

    vector<vector<double>> _distances = vector<vector<double>>(nodesSize,vector<double>(nodesSize,0)); //inicializo todas las dist en 0
    void getDistances(vector<Node> &nodes);

    Node &closestNeighbor(Node &node, int maxDemand);



public:
    Golosa(Cvrp &cvrp);
    vector<Route> algoGoloso();




};




#endif //CVRP_GOLOSA_H
