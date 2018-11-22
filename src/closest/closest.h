#ifndef CLOSEST_H_INCLUDED
#define CLOSEST_H_INCLUDED

#include "../cvrp.h"
#include <vector>

using namespace std;

class Closest
{
private:
    Cvrp _cvrp;
    vector<Route> _routes;
    vector<bool> _availableNodes;
    vector<vector<double>> _costs;

    Route* createNewRoute();
    size_t getFirstAvailableNode();
    size_t getClosestNeighbor(size_t node, int maxDemand);
public:
    Closest(Cvrp &cvrp);
    vector<Route> findBestRoutes();
};

#endif