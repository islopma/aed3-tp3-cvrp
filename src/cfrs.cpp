#include "cfrs.h"
#include <algorithm>
#include <math.h>
#include <utility>

#include <iostream>

vector<pair<double, int>> sort_by_angle(vector<Node>& nodes){
    vector<pair<double, int>> angle_ids;
    
    for(int i = 1; i < (int)nodes.size(); i++)
    {
        double y = nodes[i].y - nodes[0].y;
        double x = nodes[i].x - nodes[0].x;
        double angle = atan2(y, x);
        angle_ids.push_back({angle, i});
    }

    sort(begin(angle_ids), end(angle_ids));
    return angle_ids;
}

vector<vector<Node>> clusterize(vector<Node>& nodes, int capacity){

    auto angle_ids = sort_by_angle(nodes);
    int n = angle_ids.size();

    vector<vector<Node>> clusters;
    
    for(int i = 0; i < n; i++){
        vector<Node> cluster = {nodes[0]};
        double cluster_demand = 0;
        while(i < n && cluster_demand + nodes[angle_ids[i].second].demand <= capacity){
            cluster.push_back(nodes[angle_ids[i].second]);
            cluster_demand += nodes[angle_ids[i].second].demand;
            ++i;
        }
        clusters.push_back(cluster);
    }

    return clusters;

}

bool is_better_swap(vector<Node>& nodes, int i, int j){
    int n = nodes.size();
    
    auto current_distance = 
        hypot(nodes[i].x - nodes[(i+1)%n  ].x, nodes[i].y - nodes[(i+1)%n  ].y) + 
        hypot(nodes[j].x - nodes[(j-1+n)%n].x, nodes[j].y - nodes[(j-1+n)%n].y);

    auto new_distance = 
        hypot(nodes[j].x - nodes[(i+1)%n  ].x, nodes[j].y - nodes[(i+1)%n  ].y) + 
        hypot(nodes[i].x - nodes[(j-1+n)%n].x, nodes[i].y - nodes[(j-1+n)%n].y);

    return new_distance < current_distance;

}

void swap_edges(vector<Node>& nodes, int i, int j){
    reverse(begin(nodes)+j, begin(nodes)+i+1);
}

void route(vector<Node>& nodes){
    int n = nodes.size();
    
    bool swapped_in_last_iteration = true;

    while(swapped_in_last_iteration){
        swapped_in_last_iteration = false;
        for(int i = 0; i < n; ++i){
            for(int j = i < n-1 ? 0 : 1; j < i; ++j){
                if (is_better_swap(nodes, i, j)){
                    swap_edges(nodes, i, j);
                    swapped_in_last_iteration = true;
                }
            }
        }
    }
}