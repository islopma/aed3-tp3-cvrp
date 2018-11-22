#include "closest.h"
#include <limits>

Closest::Closest(Cvrp &cvrp)
    : _cvrp(cvrp) {
        _routes = vector<Route>();
        _availableNodes = vector<bool>(cvrp.getNodes().size(), true);
        _availableNodes[0] = false;
        _costs = cvrp.getCosts();
    }

Route* Closest::createNewRoute()
{
    auto route = Route();
    route.id = _routes.size();
    _routes.push_back(route);
    return &_routes.back();
}

size_t Closest::getFirstAvailableNode()
{
    for(size_t i = 1; i < _availableNodes.size(); i++)
    {
        if (_availableNodes[i]) return i;
    }
    return 0;
}

size_t Closest::getClosestNeighbor(size_t node, int maxDemand)
{
    size_t closest = 0;
    double minCost = numeric_limits<double>::max();
    auto &nodes = _cvrp.getNodes();
    for(size_t i = 1; i < _availableNodes.size(); i++)
    {
        if (_availableNodes[i] && i != node && _costs[node][i] < minCost && nodes[i].demand < maxDemand)
        {
            closest = i;
            minCost = _costs[node][i];
        }
    }
    return closest;
}

vector<Route> Closest::findBestRoutes()
{
    auto capacity = _cvrp.getCapacity();
    auto &nodes = _cvrp.getNodes();
    auto nodesNumber = nodes.size();

    auto route = createNewRoute();
    auto firstNode = getFirstAvailableNode();
    _cvrp.addNodeToRoute(*route, nodes[firstNode], true);
    _availableNodes[firstNode] = false;
    auto currentNode = firstNode;
    // will add n-2 nodes
    for (size_t i = 0; i < nodesNumber - 2; i++)
    {
        auto closest = getClosestNeighbor(currentNode, capacity - route->demand);
        if (closest > 0)
        {
            _cvrp.addNodeToRoute(*route, nodes[closest], true);
            _availableNodes[closest] = false;
            currentNode = closest;
        }
        else
        {
            route = createNewRoute();
            auto firstAvailableNode = getFirstAvailableNode();
            _cvrp.addNodeToRoute(*route, nodes[firstAvailableNode], true);
            _availableNodes[firstAvailableNode] = false;
            currentNode = firstAvailableNode;
        }
    }
    return _routes;
}