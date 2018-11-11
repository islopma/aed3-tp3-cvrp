#include "cfrs.h"
#include <algorithm>
#include <math.h>
#include <utility>

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