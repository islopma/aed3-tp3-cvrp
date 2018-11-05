#include <string>
#include <vector>

using namespace std;

struct Node
{
    int id;
    float x;
    float y;
    int demand;

    Node(const int id, const float x, const float y, const int demand);
};

class Cvrp
{
private:
    string _name;
    string _comment;
    int _dimension;
    int _capacity;
    vector<Node> _nodes;
    int _depotId;

    vector<string> tokenizeLine();
public:
    Cvrp();
    int readInput();
};