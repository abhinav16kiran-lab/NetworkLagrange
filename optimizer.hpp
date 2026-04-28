#ifndef OPTIMIZER_HPP
#define OPTIMIZER_HPP

struct Node{
    double lat;
    double lon;
    double weight;

    Node(double lat, double lon, double weight)
        :lat(lat), lon(lon), weight(weight) {}
};

struct Server{
    double lat;
    double lon;
    double cost;

    Server(double lat = 0, double lon = 0, double cost = 0)
        :lat(lat), lon(lon), cost(cost) {}
};

class Optimizer
{

};

#endif