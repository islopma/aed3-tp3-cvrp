#ifndef SAVINGS_H_INCLUDED
#define SAVINGS_H_INCLUDED

#include "../cvrp.h"
#include <vector>

using namespace std;

struct Saving
{
    Node first;
    Node second;
    double cost;

    Saving(const Node &first, const Node &second, const double cost);
};

class Savings
{
private:
    Cvrp _cvrp;

    vector<Saving> getSavings();
public:
    vector<Route> findBestRoutes();
};

#endif