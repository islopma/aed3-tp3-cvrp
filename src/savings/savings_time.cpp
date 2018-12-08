#include "savings.h"
#include <iostream>
#include <chrono>

using namespace std;

int main(int argc, char const *argv[])
{
    auto cvrp = Cvrp();
    auto result = cvrp.readInput();
    if (result != 0)
        return result;

    auto start = chrono::high_resolution_clock::now();

    auto savings = Savings(cvrp);
    auto routes = savings.findBestRoutes();

    auto end = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << endl;

    return 0;
}