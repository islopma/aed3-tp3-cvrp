#include "golosa.h"
#include <vector>

Golosa::Golosa(Cvrp &cvrp) {
    _cvrp = cvrp;
}

Node &Golosa::closestNeighbor(Node &node , int maxDemand) {
    int nodeID = node.id -1;
    int closest;
    bool posibleRes = true;

    int i=0;
    //encuentro uno cualquiera primero para tener como base una primera distancia "minima"
    while (i<nodesSize){
        if(i != nodeID && !_usedNodes[i] && _nodes[i].demand <= maxDemand){
            closest = i;
            break;
        }
        i++;
    }

    //me fijo que exista algun nodo cercano que pueda insertar a la ruta
    if(i == nodesSize){
        posibleRes = false;
    } else {

        //ahora busco el nodo realmente mas cercano
        for (int j = 0; j < nodesSize; j++) {
            if (!_usedNodes[j] && _distances[nodeID][j] < _distances[nodeID][closest] && _nodes[j].demand <= maxDemand) {
                closest = j;
            }
        }
    }

    if(posibleRes) {
        return _nodes[closest];
    } else {return node;}

}

bool Golosa::areThereNodesLeft() {
    for(int i=0;i<nodesSize;i++){
        if(!_usedNodes[i]){
            return true;
        }
    }
    return false;
}


void Golosa::getDistances(vector<Node>& nodes){

    for(int i=0; i<nodesSize; i++){
        int firstID = nodes[i].id;
        for(int j=0; j<nodesSize; j++){
            int secondID = nodes[j].id;
            double dist = nodes[i].distance(nodes[j]);
            _distances[firstID-1][secondID-1] = dist; //resto uno en el indice porque arranca de 1 los nodos
        }
    }

}

vector<Route> Golosa::algoGoloso() {
    vector<Route> res;
    int capacity = _cvrp.getCapacity();
    bool go = areThereNodesLeft();
    //mientras haya nodos libres sigo

    while(go){
        Route route = Route();
        Node closest = closestNeighbor(_nodes[0],capacity);      //primero quiero el mas cercano al deposito que este libre
        route.nodes.push_back(&closest);
        route.cost = _nodes[0].distance(closest);
        route.demand = closest.demand;
        _usedNodes[closest.id-1] = true;
        go = areThereNodesLeft();
        while(true){
            Node nextClosest = closestNeighbor(closest, capacity - route.demand);
            //closestNeighbor devuelve el mismo nodo si ya no tiene vecinos visitables
            if(nextClosest.id != closest.id) {
                route.nodes.push_back(&nextClosest);
                route.cost += _nodes[closest.id].distance(nextClosest);
                route.demand += nextClosest.demand;
                _usedNodes[nextClosest.id - 1] = true;
                closest = nextClosest;
                go = areThereNodesLeft();
            } else break;
            //corta cuando ya no tiene vecinos que pueda visitar

        }
        res.push_back(route);
    }

}






