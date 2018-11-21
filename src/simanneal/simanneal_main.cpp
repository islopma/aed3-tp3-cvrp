#include "simanneal.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    auto cvrp = Cvrp();
    auto result = cvrp.readInput();
    if (result != 0) return result;

    auto simanneal = Simanneal(cvrp);
    auto routes = simanneal.findBestRoutes();

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