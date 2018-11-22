#include "closest.h"
#include <iostream>
#include <chrono>

int main(int argc, char const *argv[])
{
    auto cvrp = Cvrp();
    auto result = cvrp.readInput();
    if (result != 0) return result;

    auto start = chrono::high_resolution_clock::now();

    auto closest = Closest(cvrp);
    auto routes = closest.findBestRoutes();

    auto end = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << endl;
    
    return 0;
}