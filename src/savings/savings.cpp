#include "savings.h"
#include <algorithm>

using namespace std;

Saving::Saving(Node *first, Node *second, const double cost)
    : first(first), second(second), cost(cost) {}

Savings::Savings(Cvrp &cvrp)
    : _cvrp(cvrp) {}

vector<Saving> Savings::getSavings()
{
    auto &nodes = _cvrp.getNodes();
    auto nodesNumber = nodes.size();
    auto savings = vector<Saving>();
    auto costs = _cvrp.getCosts();
    auto depotId = _cvrp.getDepotId();
    for (size_t i = 1; i < nodesNumber - 1; ++i)
    {
        auto &first = nodes[i];
        if (first.id == depotId)
            continue;
        for (size_t j = i + 1; j < nodesNumber; ++j)
        {
            auto &second = nodes[j];
            if (second.id == depotId)
                continue;
            auto savingCost = costs[depotId][first.id] + costs[depotId][second.id] - costs[first.id][second.id];
            auto saving = Saving(&first, &second, savingCost);
            savings.push_back(saving);
        }
    }
    sort(savings.begin(), savings.end(),
         [](const Saving &x, const Saving &y) -> bool {
             return x.cost > y.cost;
         });
    return savings;
}

vector<Route> Savings::findBestRoutes()
{
    auto capacity = _cvrp.getCapacity();
    auto routes = vector<Route>();
    auto &nodes = _cvrp.getNodes();
    auto nodesNumber = nodes.size();
    // will generate at most one route per node
    for (size_t i = 0; i < nodesNumber; ++i)
    {
        auto route = Route();
        route.id = i;
        routes.push_back(route);
    }
    // first mark all routes as unavailable
    auto availableRoutes = vector<bool>(nodesNumber, false);

    auto newRouteIndex = 0;
    auto savings = getSavings();
    for (auto &saving : savings)
    {
        auto &first = saving.first;
        auto &second = saving.second;
        if (first->routeId == -1 && second->routeId == -1)
        {
            // if demand exceeds capacity, skip
            if (first->demand + second->demand > capacity)
                continue;
            // create new route
            auto &route = routes[newRouteIndex++];
            availableRoutes[route.id] = true;
            _cvrp.addNodeToRoute(route, *first, true);
            _cvrp.addNodeToRoute(route, *second, true);
        }
        else if (first->routeId != -1 && second->routeId == -1)
        {
            auto &route = routes[first->routeId];
            // if demand exceeds capacity, skip
            if (route.demand + second->demand > capacity)
                continue;
            // find if first is at an end of the route, else skip
            auto frontId = route.nodes.front()->id;
            auto backId = route.nodes.back()->id;
            if (first->id != frontId && first->id != backId)
                continue;
            // add node to route
            auto isLast = first->id == backId;
            _cvrp.addNodeToRoute(route, *second, isLast);
        }
        else if (second->routeId != -1 && first->routeId == -1)
        {
            auto &route = routes[second->routeId];
            // if demand exceeds capacity, skip
            if (route.demand + first->demand > capacity)
                continue;
            // find if second is at an end of the route, else skip
            auto frontId = route.nodes.front()->id;
            auto backId = route.nodes.back()->id;
            if (second->id != frontId && second->id != backId)
                continue;
            // add node to route
            auto isLast = second->id == backId;
            _cvrp.addNodeToRoute(route, *first, isLast);
        }
        else
        {
            // if both are on same route, skip
            if (first->routeId == second->routeId)
                continue;
            auto &firstRoute = routes[first->routeId];
            auto &secondRoute = routes[second->routeId];
            // if demand exceeds capacity, skip
            if (firstRoute.demand + secondRoute.demand > capacity)
                continue;
            // find if first or second are both at an end of the routes, else skip
            auto firstFrontId = firstRoute.nodes.front()->id;
            auto firstBackId = firstRoute.nodes.back()->id;
            auto secondFrontId = secondRoute.nodes.front()->id;
            auto secondBackId = secondRoute.nodes.back()->id;
            if ((first->id != firstFrontId && first->id != firstBackId) || (second->id != secondFrontId && second->id != secondBackId))
                continue;
            // merge routes: add nodes from second to first route
            auto firstIsLast = first->id == firstBackId;
            auto secondIsLast = second->id == secondBackId;
            auto &nodesToAdd = secondRoute.nodes;
            if (secondIsLast)
            {
                // will add nodes from second route in reverse order
                reverse(nodesToAdd.begin(), nodesToAdd.end());
            }
            for (auto node : nodesToAdd)
            {
                _cvrp.addNodeToRoute(firstRoute, *node, firstIsLast);
            }
            // disable second route
            availableRoutes[secondRoute.id] = false;
        }
    }
    // create routes for single nodes
    for (auto &node : nodes)
    {
        if (node.routeId != -1 || node.id == _cvrp.getDepotId())
            continue;
        auto &route = routes[newRouteIndex++];
        availableRoutes[route.id] = true;
        _cvrp.addNodeToRoute(route, node, true);
    }
    routes.erase(remove_if(
                     routes.begin(), routes.end(),
                     [availableRoutes](const Route &x) {
                         return !availableRoutes[x.id];
                     }),
                 routes.end());
    return routes;
}