#include "cvrp.h"
#include <cmath>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>

Node::Node(const int id, const double x, const double y, const int demand)
    : id(id), x(x), y(y), demand(demand), routeId(-1) {}

double Node::distance(const Node &other)
{
    // calculate L2 norm (this - other)
    return sqrt(pow(this->x - other.x, 2) + pow(this->y - other.y, 2));
}

Route::Route()
{
    nodes = vector<Node*>();
    cost = 0;
    demand = 0;
}

Cvrp::Cvrp()
{
    _name = "";
    _comment = "";
    _dimension = 0;
    _capacity = 0;
    _nodes = vector<Node>();
    _depotId = 0;
    _costs = vector<vector<double>>();
}

vector<Node>& Cvrp::getNodes(){
    return this->_nodes;
}

int& Cvrp::getCapacity(){
    return this->_capacity;
}

int& Cvrp::getDepotId()
{
    return _depotId;
}

int Cvrp::readInput()
{
    // Specification part
    for (auto i = 0; i < 6; ++i)
    {
        auto tokens = tokenizeLine();
        if (tokens.size() < 3)
        {
            cerr << "There is an error in the specification format" << endl;
            return -1;
        }
        
        if (tokens[0] == "NAME")
        {
            _name = tokens[2];
        }
        else if (tokens[0] == "COMMENT")
        {
            _comment = "";
            for (size_t token = 2; token < tokens.size(); ++ token)
            {
                _comment += tokens[token] + " ";
            }
        }
        else if (tokens[0] == "TYPE")
        {
            if (tokens[2] != "CVRP")
            {
                cerr << "The problem is not CVRP" << endl;
                return -1;
            }
        }
        else if (tokens[0] == "DIMENSION")
        {
            _dimension = stoi(tokens[2]);
        }
        else if (tokens[0] == "EDGE_WEIGHT_TYPE")
        {
            if (tokens[2] != "EUC_2D")
            {
                cerr << "Distances are not euclidean" << endl;
                return -1;
            }
        }
        else if (tokens[0] == "CAPACITY")
        {
            _capacity = stoi(tokens[2]);
        }
        else
        {
            cerr << "Cannot recognize the keyword " << tokens[0] << endl;
            return -1;
        }
    }
    // Data part
    auto node_coord = map<int, pair<double, double>>();
    auto demand_section = map<int, int>();
    for (auto i = 0; i < 3; ++i)
    {
        auto tokens = tokenizeLine();
        if (tokens[0] == "NODE_COORD_SECTION")
        {
            for (auto node = 0; node < _dimension; ++node)
            {
                auto node_tokens = tokenizeLine();
                auto id = stoi(node_tokens[0]);
                auto x = stof(node_tokens[1]);
                auto y = stof(node_tokens[2]);
                node_coord.insert(pair<int, pair<double, double>>(id, pair<double, double>(x, y)));
            }
        }
        else if (tokens[0] == "DEMAND_SECTION")
        {
            for (auto node = 0; node < _dimension; ++node)
            {
                auto node_tokens = tokenizeLine();
                auto id = stoi(node_tokens[0]);
                auto demand = stoi(node_tokens[1]);
                demand_section.insert(pair<int, int>(id, demand));
            }
        }
        else if (tokens[0] == "DEPOT_SECTION")
        {
            auto depotTokens = tokenizeLine();
            _depotId = stoi(depotTokens[0]);
            // This should read a line with -1, as single depot is assumed
            tokenizeLine();
        }
        else
        {
            cerr << "Cannot recognize the keyword " << tokens[0] << endl;
            return -1;
        }
    }
    _nodes = vector<Node>();
    for (auto const& node : node_coord)
    {
        auto id = node.first;
        auto x = node.second.first;
        auto y = node.second.second;
        auto demand = demand_section[id];
        _nodes.push_back(Node(id, x, y, demand));
    }
    return 0;
}

vector<vector<double>>& Cvrp::getCosts()
{
    if (!_costs.empty()) return _costs;
    auto nodesNumber = _nodes.size();
    _costs = vector<vector<double>>(nodesNumber + 1, vector<double>(nodesNumber + 1, -1));
    for (size_t i = 0; i < nodesNumber - 1; ++i)
    {
        auto first = _nodes[i];
        for (size_t j = i + 1; j < nodesNumber; ++j)
        {
            auto second = _nodes[j];
            auto distance = first.distance(second);
            _costs[first.id][second.id] = distance;
            _costs[second.id][first.id] = distance;
        }
    }
    return _costs;
}

void Cvrp::addNodeToRoute(Route &route, Node &node, const bool &isLast)
{
    auto &nodes = route.nodes;
    auto isFirst = nodes.size() == 0;
    route.demand += node.demand;
    node.routeId = route.id;
    auto costs = getCosts();
    if (isFirst) {
        nodes.push_back(&node);
        route.cost += 2 * costs[_depotId][node.id];
    }
    else
    {
        if (isLast)
        {
            auto linkNode = nodes.back();
            nodes.push_back(&node);
            route.cost += costs[_depotId][node.id] + costs[linkNode->id][node.id] - costs[_depotId][linkNode->id];
        }
        else
        {
            auto linkNode = nodes.front();
            nodes.insert(nodes.begin(), &node);
            route.cost += costs[_depotId][node.id] + costs[linkNode->id][node.id] - costs[_depotId][linkNode->id];
        }
    }
}

vector<string> Cvrp::tokenizeLine()
{
    string line;
    getline(cin, line);
    istringstream buf(line);
    istream_iterator<string> beg(buf), end;
    vector<string> tokens(beg, end);
    return tokens;
}