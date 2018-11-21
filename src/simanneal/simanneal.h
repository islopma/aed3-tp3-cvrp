#ifndef SIMANNEAL_H_INCLUDED
#define SIMANNEAL_H_INCLUDED

#include "../cvrp.h"

using namespace std;

class Simanneal
{
private:
    Cvrp _cvrp;
    double T_s;
    double T_f;
    int alpha;
    int gamma;
    int R;

    double getAcceptanceProbability(double delta, double temp);
    double getUniformRandom();
    void InitParams(vector<Route> &routes);
    vector<size_t> getShuffledIndexes(size_t size);
    vector<Route> shuffleRoutes(vector<Route> &routes);
    double get10OpCost(vector<vector<double>> &costs, vector<Node*> &firstNodes,
        vector<Node*> &secondNodes, size_t firstIndex, size_t secondIndex);
    double get01OpCost(vector<vector<double>> &costs, vector<Node*> &firstNodes,
        vector<Node*> &secondNodes, size_t firstIndex, size_t secondIndex);
    double get11OpCost(vector<vector<double>> &costs, vector<Node*> &firstNodes,
        vector<Node*> &secondNodes, size_t firstIndex, size_t secondIndex);
    void moveNode(vector<Route> &routes, size_t firstRouteIndex,
        size_t secondRouteIndex, size_t firstNodeIndex, size_t secondNodeIndex);
    void swapNodes(vector<Route> &routes, size_t firstRouteIndex,
        size_t secondRouteIndex, size_t firstNodeIndex, size_t secondNodeIndex);
    void addNodeToRoute(Route &route, Node &node, size_t nextNodeIndex);
    void removeNodeFromRoute(Route &route, size_t nodeIndex);
    double getRoutesCost(vector<Route> &routes);
    double coolTemperature(double T_k, int k);
    vector<Route> copyRoutes(vector<Route> &routes);
public:
    Simanneal(Cvrp &cvrp);
    vector<Route> findBestRoutes();
};

#endif