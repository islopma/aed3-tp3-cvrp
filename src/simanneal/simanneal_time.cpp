#include "simanneal.h"
#include <iostream>
#include <chrono>

using namespace std;

int main(int argc, char const *argv[])
{
    // set parameters
    auto R = 3;
    if (argc > 1)
        R = stoi(argv[1]);
    
    auto initialSolution = "Savings";
    if (argc > 2)
        initialSolution = argv[2];
    
    auto swapOnly = false;
    if (argc > 3)
    {
        string swapArg = argv[3];
        swapOnly = swapArg == "SwapOnly";
    }
    
    auto progressMode = false;

    srand(time(NULL));
    auto cvrp = Cvrp();
    auto result = cvrp.readInput();
    if (result != 0)
        return result;

    auto start = chrono::high_resolution_clock::now();

    auto simanneal = Simanneal(cvrp, R, initialSolution, swapOnly, progressMode);
    auto routes = simanneal.findBestRoutes();

    auto end = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << endl;

    return 0;
}