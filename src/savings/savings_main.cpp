#include "savings.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    auto cvrp = Cvrp();
    auto result = cvrp.readInput();
    if (result != 0) return result;

    auto savings = Savings(cvrp);
    auto routes = savings.findBestRoutes();

    cout << routes.size() << endl;
    float totalCost = 0;
    for(auto &route : routes){
        totalCost += route.cost;
        auto nodes = route.nodes;
        for(auto node : nodes){
            cout << node->id << " ";
        }
        cout << endl;
    }
    cout << totalCost << endl;
    return 0;
}