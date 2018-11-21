#include "golosa.h"
#include <iostream>

int main(int argc, char const *argv[]){

    auto cvrp = Cvrp();
    auto result = cvrp.readInput();
    if(result != 0) return result;

    auto golosa = Golosa(cvrp);
    auto routes = golosa.algoGoloso();

    double totalCost = 0;

    for(int i=0;i<routes.size();i++){
        totalCost += routes[i].cost;
    }

    cout << routes.size() << endl;
    for(auto &route : routes){
        for(auto node : route.nodes){
            cout << node->id << " ";
        }
        cout << endl;
    }

    cout << totalCost << endl;

    return 0;


}
