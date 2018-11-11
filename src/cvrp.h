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

    Node(const int id, const double x, const double y, const int demand);
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

    vector<string> tokenizeLine();
public:
    Cvrp();
    int readInput();
    vector<Node> getNodes();
    int getCapacity();
};

#endif