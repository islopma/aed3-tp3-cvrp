#ifndef SAVINGS_H_INCLUDED
#define SAVINGS_H_INCLUDED

#include "../cvrp.h"
#include <vector>

using namespace std;

struct Saving
{
    Node *first;
    Node *second;
    double cost;

    Saving(Node *first, Node *second, const double cost);
};

class Savings
{
private:
    Cvrp _cvrp;

    vector<Saving> getSavings();
public:
    Savings(Cvrp &cvrp);
    vector<Route> findBestRoutes();
};

#endif