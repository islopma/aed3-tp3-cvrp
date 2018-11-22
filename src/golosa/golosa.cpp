#include "golosa.h"
#include <vector>
#include <algorithm>


void getUsedNodes(vector<bool> &_usedNodes, int nodesSize) {
    _usedNodes.push_back(true);                 //al deposito lo marco como usado
    for(int v=1;v<nodesSize;v++){
        _usedNodes.push_back(false);
    }
}


int closestNeighbor(Node &node , int maxDemand, vector<vector<double>> &_distances, vector<bool> _usedNodes, vector<Node> &_nodes, int candidato) {
    int nodeID = node.id -1;
    int nodesSize = _nodes.size();
    int closest = candidato;



     //ahora busco el nodo realmente mas cercano
    for (int j = 0; j < nodesSize; j++) {
        if (!_usedNodes[j] && _distances[nodeID][j] < _distances[nodeID][closest] && _nodes[j].demand <= maxDemand) {
                closest = j;
        }
    }


    return closest;

}

bool areThereNodesLeft(vector<bool> &_usedNodes) {
    int nodesSize = _usedNodes.size();
    //me fijo si quedan nodos disponibles ignorando el deposito

    for(int i=1;i<nodesSize;i++){
        if(!_usedNodes[i]){
            return true;
        }
    }
    return false;
}

//devuelve el indice de un nodo que este libre y que sea candidato a ser vecino mas cercano con demanda apropiada

int areThereNodesWithSmallerDemand(Node &node , int maxDemand, vector<bool> _usedNodes, vector<Node> &_nodes){
    int nodeID = node.id -1;
    int nodesSize = _nodes.size();

    for(int i=0;i<nodesSize;i++){
        if(i != nodeID){
            if(!_usedNodes[i]){
                if(_nodes[i].demand <= maxDemand){
                    return i;
                }
            }
        }
    }
    return -1;

}

vector<vector<double>> getDistances(vector<Node>& nodes){
    vector<vector<double>> _distances;
    int nodesSize = nodes.size();

    for(int m=0;m<nodesSize;m++){
        for(int n=0;n<nodesSize;n++){
            _distances[m].push_back(0);
        }
    }

    for(int i=0; i<nodesSize; i++){
        int firstID = nodes[i].id;
        for(int j=0; j<nodesSize; j++){
            int secondID = nodes[j].id;
            double dist = nodes[i].distance(nodes[j]);
            _distances[firstID-1][secondID-1] = dist; //resto uno en el indice porque arranca de 1 los nodos
        }
    }
    return _distances;
}


void addNodeToRuta(Route &route, Node &node, const bool &isLast, vector<vector<double>> costs){

    auto &nodes = route.nodes;
    auto isFirst = nodes.size() == 0;
    route.demand += node.demand;
    node.routeId = route.id;
    if (isFirst) {
        nodes.push_back(&node);
        route.cost += 2 * costs[0][node.id];
    }
    else
    {
        if (isLast)
        {
            auto linkNode = nodes.back();
            nodes.push_back(&node);
            route.cost += costs[0][node.id] + costs[linkNode->id][node.id] - costs[0][linkNode->id];
        }
        else
        {
            auto linkNode = nodes.front();
            nodes.insert(nodes.begin(), &node);
            route.cost += costs[0][node.id] + costs[linkNode->id][node.id] - costs[0][linkNode->id];
        }
    }

}



vector<Route> algoGoloso(vector<Node> &nodes, int capacity, Cvrp &_cvrp) {
    vector<Node> _nodes = nodes;

    vector<bool> _usedNodes;
    getUsedNodes(_usedNodes, _nodes.size());

    vector<vector<double>> _distances = getDistances(_nodes);
    vector<Route> res;
    //mientras haya nodos libres sigo

    while(true){

        if(!areThereNodesLeft(_usedNodes)){break;}
        int candidato = areThereNodesWithSmallerDemand(_nodes[0],capacity,_usedNodes,_nodes);
        Route route = Route();

        int indClosest = closestNeighbor(_nodes[0],capacity,_distances,_usedNodes,_nodes, candidato);   //primero quiero el mas cercano al deposito que este libre
        Node closest = _nodes[indClosest];
        addNodeToRuta(route,closest,true,_distances);                //por ahora es el ultimo



        _usedNodes[indClosest] = true;


        if(!areThereNodesLeft(_usedNodes)){
            res.push_back(route);
            break;
        }

        while(true) {
            int candidate = areThereNodesWithSmallerDemand(closest, capacity - route.demand, _usedNodes, _nodes);
            if (candidate==-1) {
                break;
            } else {
                int nextClosest = closestNeighbor(closest, capacity - route.demand, _distances, _usedNodes, _nodes,candidate);

                //closestNeighbor devuelve el mismo nodo si ya no tiene vecinos visitables

                addNodeToRuta(route, _nodes[nextClosest], true,_distances);
                _usedNodes[nextClosest] = true;
                indClosest = nextClosest;
                closest = _nodes[indClosest];
                if (!areThereNodesLeft(_usedNodes)) {
                    break;
                }

                //corta cuando ya no tiene vecinos que pueda visitar

                }
        }
        res.push_back(route);


    }

    return res;

}






