#include "savings.h"

Saving::Saving(const Node &first, const Node &second, const double cost)
    : first(first), second(second), cost(cost) {}

vector<Saving> Savings::getSavings()
{
    return vector<Saving>();
}

vector<Route> Savings::findBestRoutes()
{
    return vector<Route>();
}