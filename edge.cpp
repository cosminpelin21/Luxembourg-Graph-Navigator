#include "edge.h"

Edge::Edge() : toNodeId(0), length(0.0) {}
Edge::Edge(int toNodeId, double length) : toNodeId(toNodeId), length(length) {}
int Edge::getToNodeId() const
{
    return toNodeId;
}
double Edge::getLength() const
{
    return length;
}
