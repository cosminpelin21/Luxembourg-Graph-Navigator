#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <QString>
#include "node.h"
#include "edge.h"
#include "kdtree.h"
#include <unordered_map>
class Graph
{
public:
    Graph();
    void loadFromXML(const QString& filePath);
    std::vector<int> dijkstra(int startId, int endId);
    int getClosestNode(double x, double y, double width, double height);

    const std::unordered_map<int, Node>& getNodes() const;
    const std::unordered_map<int, std::vector<Edge>>& getAdjacencyList() const;

    double getMinLat() const;
    double getMaxLat() const;
    double getMinLon() const;
    double getMaxLon() const;

private:
    std::unordered_map<int, Node> nodes;
    std::unordered_map<int, std::vector<Edge>> adjacencyList;
    KDTree kdTree;
    double minLat, maxLat, minLon, maxLon;
};

#endif // GRAPH_H
