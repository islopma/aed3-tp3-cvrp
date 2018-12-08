#include "cvrp.h"
#include "cfrs.h"

#include <iostream>
#include <chrono>

using namespace std;

int main(int argc, char const *argv[])
{
    auto cvrp = Cvrp();
    cvrp.readInput();

    auto start = chrono::high_resolution_clock::now();

    auto nodes = cvrp.getNodes();
    int capacity = cvrp.getCapacity();

    auto clusters = clusterize(nodes, capacity);

    double total_cost = 0;

    for (int i = 0; i < (int)clusters.size(); ++i)
    {
        route(clusters[i]);
        total_cost += get_cost(clusters[i]);
    }

    auto end = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << endl;

    return 0;
}