#include "cvrp.h"
#include "cfrs.h"

#include <iostream>

int main(int argc, char const *argv[])
{
    auto cvrp = Cvrp();
    cvrp.readInput();

    auto nodes = cvrp.getNodes();
    int capacity = cvrp.getCapacity();
    
    auto clusters = clusterize(nodes, capacity);

    double total_cost = 0;

    for(int i = 0; i < (int) clusters.size(); ++i){
        route(clusters[i]);
        total_cost += get_cost(clusters[i]);
    }

    cout << clusters.size() << endl;
    for(int i = 0; i < (int) clusters.size(); ++i){        
        for(int j = 1; j < (int) clusters[i].size(); ++j){
            cout << clusters[i][j].id << " \n"[j+1 == (int) clusters[i].size()];
        }
    }
    cout << total_cost << endl;

    return 0;
}