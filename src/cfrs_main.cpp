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

    for(int i = 0; i < (int) clusters.size(); ++i){
        for(auto &node: clusters[i]){
            std::cout << node.x << " " << node.y << " " << i << std::endl;
        }
        std::cout << nodes[0].x << " " << nodes[0].y << " " << i << std::endl;
    }
    return 0;
}