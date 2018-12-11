#include "simanneal.h"
#include <iostream>

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
    if (argc > 4)
    {
        string progressArg = argv[4];
        progressMode = progressArg == "ProgressMode";
    }

    srand(time(NULL));
    auto cvrp = Cvrp();
    auto result = cvrp.readInput();
    if (result != 0)
        return result;

    auto simanneal = Simanneal(cvrp, R, initialSolution, swapOnly, progressMode);
    auto routes = simanneal.findBestRoutes();
    
    if (progressMode)
        return 0;

    cout << routes.size() << endl;
    float totalCost = 0;
    for (auto &route : routes)
    {
        totalCost += route.cost;
        auto nodes = route.nodes;
        for (auto node : nodes)
        {
            cout << node->id << " ";
        }
        cout << endl;
    }
    cout << totalCost << endl;
    return 0;
}