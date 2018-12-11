#include "simanneal.h"
#include "../savings/savings.h"
#include "../closest/closest.h"
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>

using namespace std;

Simanneal::Simanneal(Cvrp &cvrp)
    : Simanneal(cvrp, 3, "Savings", false, false) {}

Simanneal::Simanneal(Cvrp &cvrp, int R, std::string initialSolution, bool swapOnly, bool progressMode)
    : _cvrp(cvrp), R(R), _initialSolution(initialSolution), _swapOnly(swapOnly), _progressMode(progressMode)
{
    T_s = 0;
    T_f = INT_MAX;
    alpha = 0;
    gamma = 0;
}

double Simanneal::getAcceptanceProbability(double delta, double temp)
{
    auto ap = exp(-delta / temp);
    return ap;
}

double Simanneal::getUniformRandom()
{
    auto uniform = (double)rand() / (RAND_MAX + 1.0);
    return uniform;
}

void Simanneal::InitParams(vector<Route> &routes)
{
    auto &costs = _cvrp.getCosts();
    auto capacity = _cvrp.getCapacity();
    auto shuffledRoutes = shuffleRoutes(routes);
    auto Nfeas = 0;
    for (size_t i = 0; i < shuffledRoutes.size() - 1; ++i)
    {
        auto firstRoute = shuffledRoutes[i];
        auto firstNodes = firstRoute.nodes;
        for (size_t j = i + 1; j < shuffledRoutes.size(); ++j)
        {
            auto secondRoute = shuffledRoutes[j];
            auto secondNodes = secondRoute.nodes;
            for (size_t k = 0; k < firstNodes.size(); ++k)
            {
                auto firstNode = firstNodes[k];
                for (size_t l = 0; l < secondNodes.size(); ++l)
                {
                    auto secondNode = secondNodes[l];
                    // (1,0) operator, insert firstNode before secondNode
                    if (!_swapOnly && firstNode->demand + secondRoute.demand <= capacity)
                    {
                        auto delta = get10OpCost(costs, firstNodes, secondNodes, k, l);
                        auto absDelta = abs(delta);
                        if (absDelta > T_s)
                            T_s = absDelta;
                        if (absDelta < T_f)
                            T_f = absDelta;
                        ++Nfeas;
                    }
                    // (0,1) operator, insert secondNode before firstNode
                    if (!_swapOnly && secondNode->demand + firstRoute.demand <= capacity)
                    {
                        auto delta = get01OpCost(costs, firstNodes, secondNodes, k, l);
                        auto absDelta = abs(delta);
                        if (absDelta > T_s)
                            T_s = absDelta;
                        if (absDelta < T_f)
                            T_f = absDelta;
                        ++Nfeas;
                    }
                    // (1,1) operator, swap firstNode and secondNode
                    if (firstNode->demand + secondRoute.demand - secondNode->demand <= capacity && secondNode->demand + firstRoute.demand - firstNode->demand <= capacity)
                    {
                        auto delta = get11OpCost(costs, firstNodes, secondNodes, k, l);
                        auto absDelta = abs(delta);
                        if (absDelta > T_s)
                            T_s = absDelta;
                        if (absDelta < T_f)
                            T_f = absDelta;
                        ++Nfeas;
                    }
                }
            }
        }
    }
    auto customersNumber = _cvrp.getNodes().size() - 1;
    alpha = customersNumber * Nfeas;
    gamma = customersNumber;
}

vector<size_t> Simanneal::getShuffledIndexes(size_t size)
{
    auto indexes = vector<size_t>(size);
    for (size_t index = 0; index < size; ++index)
    {
        indexes[index] = index;
    }
    random_shuffle(indexes.begin(), indexes.end());
    return indexes;
}

vector<Route> Simanneal::shuffleRoutes(vector<Route> &routes)
{
    auto shuffledRoutes = routes;
    random_shuffle(shuffledRoutes.begin(), shuffledRoutes.end());
    return shuffledRoutes;
}

double Simanneal::get10OpCost(vector<vector<double>> &costs, vector<Node *> &firstNodes,
                              vector<Node *> &secondNodes, size_t firstIndex, size_t secondIndex)
{
    auto depotId = _cvrp.getDepotId();
    auto firstNode = firstNodes[firstIndex];
    auto secondNode = secondNodes[secondIndex];
    auto prevFirstId = firstIndex == 0 ? depotId : firstNodes[firstIndex - 1]->id;
    auto nextFirstId = firstIndex == firstNodes.size() - 1 ? depotId : firstNodes[firstIndex + 1]->id;
    auto prevSecondId = secondIndex == 0 ? depotId : secondNodes[secondIndex - 1]->id;
    auto delta = -costs[prevFirstId][firstNode->id] - costs[firstNode->id][nextFirstId] + costs[prevFirstId][nextFirstId] - costs[prevSecondId][secondNode->id] + costs[prevSecondId][firstNode->id] + costs[firstNode->id][secondNode->id];
    return delta;
}

double Simanneal::get01OpCost(vector<vector<double>> &costs, vector<Node *> &firstNodes,
                              vector<Node *> &secondNodes, size_t firstIndex, size_t secondIndex)
{
    auto depotId = _cvrp.getDepotId();
    auto firstNode = firstNodes[firstIndex];
    auto secondNode = secondNodes[secondIndex];
    auto prevFirstId = firstIndex == 0 ? depotId : firstNodes[firstIndex - 1]->id;
    auto prevSecondId = secondIndex == 0 ? depotId : secondNodes[secondIndex - 1]->id;
    auto nextSecondId = secondIndex == secondNodes.size() - 1 ? depotId : secondNodes[secondIndex + 1]->id;
    auto delta = -costs[prevSecondId][secondNode->id] - costs[secondNode->id][nextSecondId] + costs[prevSecondId][nextSecondId] - costs[prevFirstId][firstNode->id] + costs[prevFirstId][secondNode->id] + costs[secondNode->id][firstNode->id];
    return delta;
}

double Simanneal::get11OpCost(vector<vector<double>> &costs, vector<Node *> &firstNodes,
                              vector<Node *> &secondNodes, size_t firstIndex, size_t secondIndex)
{
    auto depotId = _cvrp.getDepotId();
    auto firstNode = firstNodes[firstIndex];
    auto secondNode = secondNodes[secondIndex];
    auto prevFirstId = firstIndex == 0 ? depotId : firstNodes[firstIndex - 1]->id;
    auto nextFirstId = firstIndex == firstNodes.size() - 1 ? depotId : firstNodes[firstIndex + 1]->id;
    auto prevSecondId = secondIndex == 0 ? depotId : secondNodes[secondIndex - 1]->id;
    auto nextSecondId = secondIndex == secondNodes.size() - 1 ? depotId : secondNodes[secondIndex + 1]->id;
    auto delta = -costs[prevFirstId][firstNode->id] - costs[firstNode->id][nextFirstId] + costs[prevFirstId][secondNode->id] + costs[secondNode->id][nextFirstId] - costs[prevSecondId][secondNode->id] - costs[secondNode->id][nextSecondId] + costs[prevSecondId][firstNode->id] + costs[firstNode->id][nextSecondId];
    return delta;
}

void Simanneal::moveNode(vector<Route> &routes, size_t firstRouteIndex,
                         size_t secondRouteIndex, size_t firstNodeIndex, size_t secondNodeIndex)
{
    // insert firstNode before secondNode
    auto &firstRoute = routes[firstRouteIndex];
    auto &secondRoute = routes[secondRouteIndex];
    auto &firstNodes = firstRoute.nodes;
    auto firstNode = firstNodes[firstNodeIndex];
    addNodeToRoute(secondRoute, *firstNode, secondNodeIndex);
    removeNodeFromRoute(firstRoute, firstNodeIndex);
    // if firstRoute is empty, remove it as it can no longer be used
    if (firstRoute.nodes.size() == 0)
        routes.erase(routes.begin() + firstRouteIndex);
}

void Simanneal::swapNodes(vector<Route> &routes, size_t firstRouteIndex,
                          size_t secondRouteIndex, size_t firstNodeIndex, size_t secondNodeIndex)
{
    auto &firstRoute = routes[firstRouteIndex];
    auto &secondRoute = routes[secondRouteIndex];
    auto &firstNodes = firstRoute.nodes;
    auto &secondNodes = secondRoute.nodes;
    auto firstNode = firstNodes[firstNodeIndex];
    auto secondNode = secondNodes[secondNodeIndex];
    auto prevFirstId = firstNodeIndex == 0 ? _cvrp.getDepotId() : firstNodes.at(firstNodeIndex - 1)->id;
    auto nextFirstId = firstNodeIndex == firstNodes.size() - 1 ? _cvrp.getDepotId() : firstNodes.at(firstNodeIndex + 1)->id;
    auto prevSecondId = secondNodeIndex == 0 ? _cvrp.getDepotId() : secondNodes.at(secondNodeIndex - 1)->id;
    auto nextSecondId = secondNodeIndex == secondNodes.size() - 1 ? _cvrp.getDepotId() : secondNodes.at(secondNodeIndex + 1)->id;
    firstRoute.demand += secondNode->demand - firstNode->demand;
    secondRoute.demand += firstNode->demand - secondNode->demand;
    auto &costs = _cvrp.getCosts();
    firstRoute.cost += costs[prevFirstId][secondNode->id] + costs[secondNode->id][nextFirstId] - costs[prevFirstId][firstNode->id] - costs[firstNode->id][nextFirstId];
    secondRoute.cost += costs[prevSecondId][firstNode->id] + costs[firstNode->id][nextSecondId] - costs[prevSecondId][secondNode->id] - costs[secondNode->id][nextSecondId];
    firstNodes[firstNodeIndex] = secondNode;
    secondNodes[secondNodeIndex] = firstNode;
}

void Simanneal::addNodeToRoute(Route &route, Node &node, size_t nextNodeIndex)
{
    auto &nodes = route.nodes;
    route.demand += node.demand;
    node.routeId = route.id;
    auto nextNode = nodes.at(nextNodeIndex);
    nodes.insert(nodes.begin() + nextNodeIndex, &node);
    auto prevId = nextNodeIndex == 0 ? _cvrp.getDepotId() : nodes.at(nextNodeIndex - 1)->id;
    auto &costs = _cvrp.getCosts();
    route.cost += costs[prevId][node.id] + costs[node.id][nextNode->id] - costs[prevId][nextNode->id];
}

void Simanneal::removeNodeFromRoute(Route &route, size_t nodeIndex)
{
    auto &nodes = route.nodes;
    auto &node = nodes[nodeIndex];
    route.demand -= node->demand;
    auto prevId = nodeIndex == 0 ? _cvrp.getDepotId() : nodes.at(nodeIndex - 1)->id;
    auto nextId = nodeIndex == nodes.size() - 1 ? _cvrp.getDepotId() : nodes.at(nodeIndex + 1)->id;
    auto &costs = _cvrp.getCosts();
    route.cost -= costs[prevId][node->id] + costs[node->id][nextId] - costs[prevId][nextId];
    nodes.erase(nodes.begin() + nodeIndex);
}

double Simanneal::getRoutesCost(vector<Route> &routes)
{
    auto cost = 0;
    for (auto &route : routes)
    {
        cost += route.cost;
    }
    return cost;
}

double Simanneal::coolTemperature(double T_k, int k)
{
    auto B_k = (T_s - T_f) / ((alpha + gamma * sqrt(k)) * T_s * T_f);
    return T_k / (1 + B_k * T_k);
}

vector<Route> Simanneal::copyRoutes(vector<Route> &routes)
{
    auto newRoutes = vector<Route>();
    for (auto &route : routes)
    {
        auto newRoute = Route();
        newRoute.id = route.id;
        newRoute.demand = route.demand;
        newRoute.cost = route.cost;
        newRoute.nodes = vector<Node *>();
        for (auto node : route.nodes)
        {
            auto newNode = new Node(node->id, node->x, node->y, node->demand);
            newNode->routeId = node->routeId;
            newRoute.nodes.push_back(newNode);
        }
        newRoutes.push_back(newRoute);
    }
    return newRoutes;
}

vector<Route> Simanneal::findBestRoutes()
{
    vector<Route> routes;
    if (_initialSolution == "Savings")
    {
        auto savings = Savings(_cvrp);
        auto bestRoutes = savings.findBestRoutes();
        routes = copyRoutes(bestRoutes);
    }
    else if (_initialSolution == "Closest")
    {
        auto closest = Closest(_cvrp);
        auto bestRoutes = closest.findBestRoutes();
        routes = copyRoutes(bestRoutes);
    } else {
        cerr << "Invalid initial solution: " << _initialSolution << endl;
        return vector<Route>();
    }

    InitParams(routes);

    // T_r is the reset temperature
    auto T_r = T_s;
    // S_b contains best solution
    auto S_b = copyRoutes(routes);
    // T_b is the temperature of the best solution
    auto T_b = T_s;
    // C_b is the cost of the best solution
    auto C_b = getRoutesCost(S_b);

    auto k = 1;
    auto resets = 0;
    auto T_k = T_s;
    auto capacity = _cvrp.getCapacity();
    auto &costs = _cvrp.getCosts();

    while (resets < R)
    {
        if (_progressMode)
            cout << T_k << " " << getRoutesCost(routes) << endl;
        auto solutionAccepted = false;
        auto routesIndexes = getShuffledIndexes(routes.size());
        for (size_t i = 0; i < routesIndexes.size() - 1; ++i)
        {
            if (solutionAccepted)
                break;
            auto firstRouteIndex = routesIndexes[i];
            auto firstRoute = routes[firstRouteIndex];
            auto firstNodes = firstRoute.nodes;
            for (size_t j = i + 1; j < routesIndexes.size(); ++j)
            {
                if (solutionAccepted)
                    break;
                auto secondRouteIndex = routesIndexes[j];
                auto secondRoute = routes[secondRouteIndex];
                auto secondNodes = secondRoute.nodes;
                auto firstNodesIndexes = getShuffledIndexes(firstNodes.size());
                auto secondNodesIndexes = getShuffledIndexes(secondNodes.size());
                for (size_t firstNodeIndex : firstNodesIndexes)
                {
                    if (solutionAccepted)
                        break;
                    auto firstNode = firstNodes[firstNodeIndex];
                    for (size_t secondNodeIndex : secondNodesIndexes)
                    {
                        auto secondNode = secondNodes[secondNodeIndex];
                        // (1,0) operator, insert firstNode before secondNode
                        if (!_swapOnly && firstNode->demand + secondRoute.demand <= capacity)
                        {
                            auto delta = get10OpCost(costs, firstNodes, secondNodes, firstNodeIndex, secondNodeIndex);
                            if (delta <= 0 || getAcceptanceProbability(delta, T_k) >= getUniformRandom())
                            {
                                moveNode(routes, firstRouteIndex, secondRouteIndex, firstNodeIndex, secondNodeIndex);
                                auto newCost = getRoutesCost(routes);
                                if (newCost < C_b)
                                {
                                    C_b = newCost;
                                    S_b = copyRoutes(routes);
                                    T_b = T_k;
                                    resets = 0;
                                }
                                solutionAccepted = true;
                                T_k = coolTemperature(T_k, k);
                                ++k;
                                break;
                            }
                        }
                        // (0,1) operator, insert secondNode before firstNode
                        if (!_swapOnly && secondNode->demand + firstRoute.demand <= capacity)
                        {
                            auto delta = get01OpCost(costs, firstNodes, secondNodes, firstNodeIndex, secondNodeIndex);
                            if (delta <= 0 || getAcceptanceProbability(delta, T_k) >= getUniformRandom())
                            {
                                moveNode(routes, secondRouteIndex, firstRouteIndex, secondNodeIndex, firstNodeIndex);
                                auto newCost = getRoutesCost(routes);
                                if (newCost < C_b)
                                {
                                    C_b = newCost;
                                    S_b = copyRoutes(routes);
                                    T_b = T_k;
                                    resets = 0;
                                }
                                solutionAccepted = true;
                                T_k = coolTemperature(T_k, k);
                                ++k;
                                break;
                            }
                        }
                        // (1,1) operator, swap firstNode and secondNode
                        if (firstNode->demand + secondRoute.demand - secondNode->demand <= capacity && secondNode->demand + firstRoute.demand - firstNode->demand <= capacity)
                        {
                            auto delta = get11OpCost(costs, firstNodes, secondNodes, firstNodeIndex, secondNodeIndex);
                            if (delta <= 0 || getAcceptanceProbability(delta, T_k) >= getUniformRandom())
                            {
                                swapNodes(routes, firstRouteIndex, secondRouteIndex, firstNodeIndex, secondNodeIndex);
                                auto newCost = getRoutesCost(routes);
                                if (newCost < C_b)
                                {
                                    C_b = newCost;
                                    S_b = copyRoutes(routes);
                                    T_b = T_k;
                                    resets = 0;
                                }
                                solutionAccepted = true;
                                T_k = coolTemperature(T_k, k);
                                ++k;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if (solutionAccepted)
            continue;
        // end cycle search with increment rule
        T_r = max(T_r / 2, T_b);
        T_k = T_r;
        ++resets;
        if (_progressMode)
            cout << "RESET" << endl;
    }

    return S_b;
}