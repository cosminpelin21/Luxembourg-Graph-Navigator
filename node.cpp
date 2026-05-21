#include "node.h"

Node::Node() : id(0), lat(0.0), lon(0.0){}
Node::Node(int id, double lat, double lon) : id(id), lat(lat), lon(lon) {}
int Node::getId() const
{
    return id;
}
double Node::getLat() const
{
    return lat;
}
double Node::getLon() const
{
    return lon;
}
