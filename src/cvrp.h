#ifndef CVRP_H_INCLUDED
#define CVRP_H_INCLUDED

#include <string>
#include <vector>

using namespace std;

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
    vector<Node*> nodes;
    double cost;
    int demand;

    Route();
};

class Cvrp
{
private:
    string _name;
    string _comment;
    int _dimension;
    int _capacity;
    vector<Node> _nodes;
    int _depotId;
    vector<vector<double>> _costs;

    vector<string> tokenizeLine();
public:
    Cvrp();
    int readInput();
    vector<Node>& getNodes();
    int& getCapacity();
    int& getDepotId();
    vector<vector<double>>& getCosts();
    void addNodeToRoute(Route &route, Node &node, const bool &isLast);
};

#endif