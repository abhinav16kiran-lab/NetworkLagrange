#pragma once
#include <vector>
#include <cmath>
#include <string>

struct Node {
    double lat;
    double lon;
    double weight;
    Node(double lat, double lon, double weight)
        : lat(lat), lon(lon), weight(weight) {}
};

struct Server {
    double lat;
    double lon;
    double cost;
    Server(double lat = 20.0, double lon = 20.0, double cost = 0.0)
        : lat(lat), lon(lon), cost(cost) {}
};

class Optimizer {
private:
    const std::vector<Node>& nodes;
    double tolerance;
    int    max_iterations;

    double haversine_distance(double lat1, double lon1,
                               double lat2, double lon2) const;
    void   compute_gradient(double lat, double lon,
                             double& grad_lat, double& grad_lon) const;

public:
    double compute_cost(double lat, double lon) const;
    Optimizer(const std::vector<Node>& node_list);
    Server find_optimal_server();
};