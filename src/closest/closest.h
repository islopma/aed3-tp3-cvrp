#ifndef CLOSEST_H_INCLUDED
#define CLOSEST_H_INCLUDED

#include "../cvrp.h"
#include <vector>

class Closest
{
  private:
    Cvrp _cvrp;
    std::vector<Route> _routes;
    std::vector<bool> _availableNodes;
    std::vector<std::vector<double>> _costs;

    Route *createNewRoute();
    size_t getFirstAvailableNode();
    size_t getClosestNeighbor(size_t node, int maxDemand);

  public:
    Closest(Cvrp &cvrp);
    std::vector<Route> findBestRoutes();
};

#endif