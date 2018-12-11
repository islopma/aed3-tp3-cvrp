#ifndef SIMANNEAL_H_INCLUDED
#define SIMANNEAL_H_INCLUDED

#include "../cvrp.h"
#include <string>

class Simanneal
{
  private:
    Cvrp _cvrp;
    double T_s;
    double T_f;
    int alpha;
    int gamma;
    int R;
    std::string _initialSolution;
    bool _swapOnly;
    bool _progressMode;

    double getAcceptanceProbability(double delta, double temp);
    double getUniformRandom();
    void InitParams(std::vector<Route> &routes);
    std::vector<size_t> getShuffledIndexes(size_t size);
    std::vector<Route> shuffleRoutes(std::vector<Route> &routes);
    double get10OpCost(std::vector<std::vector<double>> &costs, std::vector<Node *> &firstNodes,
                       std::vector<Node *> &secondNodes, size_t firstIndex, size_t secondIndex);
    double get01OpCost(std::vector<std::vector<double>> &costs, std::vector<Node *> &firstNodes,
                       std::vector<Node *> &secondNodes, size_t firstIndex, size_t secondIndex);
    double get11OpCost(std::vector<std::vector<double>> &costs, std::vector<Node *> &firstNodes,
                       std::vector<Node *> &secondNodes, size_t firstIndex, size_t secondIndex);
    void moveNode(std::vector<Route> &routes, size_t firstRouteIndex,
                  size_t secondRouteIndex, size_t firstNodeIndex, size_t secondNodeIndex);
    void swapNodes(std::vector<Route> &routes, size_t firstRouteIndex,
                   size_t secondRouteIndex, size_t firstNodeIndex, size_t secondNodeIndex);
    void addNodeToRoute(Route &route, Node &node, size_t nextNodeIndex);
    void removeNodeFromRoute(Route &route, size_t nodeIndex);
    double getRoutesCost(std::vector<Route> &routes);
    double coolTemperature(double T_k, int k);
    std::vector<Route> copyRoutes(std::vector<Route> &routes);

  public:
    Simanneal(Cvrp &cvrp);
    Simanneal(Cvrp &cvrp, int R, std::string initialSolution, bool swapOnly, bool progressMode);
    std::vector<Route> findBestRoutes();
};

#endif