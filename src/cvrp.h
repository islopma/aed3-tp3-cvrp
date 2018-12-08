#ifndef CVRP_H_INCLUDED
#define CVRP_H_INCLUDED

#include <string>
#include <vector>

struct Node
{
    int id;
    double x;
    double y;
    int demand;
    int routeId;

    Node(const int id, const double x, const double y, const int demand);
    double distance(const Node &other);
};

struct Route
{
    int id;
    std::vector<Node *> nodes;
    double cost;
    int demand;

    Route();
};

class Cvrp
{
  private:
    std::string _name;
    std::string _comment;
    int _dimension;
    int _capacity;
    std::vector<Node> _nodes;
    int _depotId;
    std::vector<std::vector<double>> _costs;

    std::vector<std::string> tokenizeLine();

  public:
    Cvrp();
    int readInput();
    std::vector<Node> &getNodes();
    int &getCapacity();
    int &getDepotId();
    std::vector<std::vector<double>> &getCosts();
    void addNodeToRoute(Route &route, Node &node, const bool &isLast);
};

#endif