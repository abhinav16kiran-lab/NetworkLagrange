#include "optimizer.hpp"
#include <iostream>


static constexpr double PI = 3.14159265358979323846;

Optimizer::Optimizer(const std::vector<Node>& node_list)
    : nodes(node_list), tolerance(1e-6), max_iterations(100000) {}

double Optimizer::haversine_distance(double lat1, double lon1,
                                      double lat2, double lon2) const {
    const double R = 6371.0;
    double r_lat1 = lat1 * PI / 180.0;//converting degrees into radians
    double r_lat2 = lat2 * PI / 180.0;
    double diff_lat = (lat2 - lat1) * PI / 180.0;
    double diff_lon = (lon2 - lon1) * PI / 180.0;

    double a = std::sin(diff_lat / 2) * std::sin(diff_lat / 2) +
               std::cos(r_lat1) * std::cos(r_lat2) *
               std::sin(diff_lon / 2) * std::sin(diff_lon / 2);

    return R * 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));//converting radians into kilometers
}

double Optimizer::compute_cost(double lat, double lon) const {
    double total = 0.0;
    for (const auto& node : nodes)
        total += node.weight * haversine_distance(lat, lon, node.lat, node.lon);
    return total;
}

void Optimizer::compute_gradient(double lat, double lon,
                                   double& grad_lat, double& grad_lon) const {
    // Calculate total weight to normalize the gradient magnitude
    double total_weight = 0;
    for (const auto& n : nodes) total_weight += n.weight;
        const double h = 0.001; // Smaller delta for better precision
    
    
    double scale = total_weight * 111.0;
    
    grad_lat = (compute_cost(lat + h, lon) - compute_cost(lat - h, lon)) / (2.0 * h * scale);
    grad_lon = (compute_cost(lat, lon + h) - compute_cost(lat, lon - h)) / (2.0 * h * scale);
}

Server Optimizer::find_optimal_server() {
    double total_weight = 0.0;
    double start_lat    = 0.0;
    double start_lon    = 0.0;

    for (const auto& node : nodes) {//finding weighted centre of all the cities
        start_lat    += node.weight * node.lat;
        start_lon    += node.weight * node.lon;
        total_weight += node.weight;
    }
    start_lat /= total_weight;
    start_lon /= total_weight;

    std::cout << "Weighted centroid start: (" << start_lat << ", " << start_lon << ")" << std::endl;

    Server best(start_lat, start_lon, 0.0);
    double step = 0.5;
    int iterations = 0;

    while (iterations < 3000) { 
        double grad_lat, grad_lon;
        compute_gradient(best.lat, best.lon, grad_lat, grad_lon);

        
        best.lat -= step * grad_lat;
        best.lon -= step * grad_lon;

        
        best.lat = std::max(-90.0,  std::min(90.0,  best.lat));
        best.lon = std::max(-180.0, std::min(180.0, best.lon));

        if (iterations % 500 == 0) {
            std::cout << "Iter " << iterations
                      << " | lat: " << best.lat
                      << " lon: "   << best.lon
                      << " step: "  << step << std::endl;
        }

        step *= 0.995; // Decay the learning rate
        iterations++;
        
       
        if (std::abs(grad_lat) < 1e-7 && std::abs(grad_lon) < 1e-7) break;
    }

    best.cost = compute_cost(best.lat, best.lon);
    return best;
}