#include "graph.h"
#include <QFile>
#include <QXmlStreamReader>
#include <limits>
#include <queue>
#include <cmath>
#include <functional>
#include <unordered_map>

Graph::Graph()
{
    minLat = std::numeric_limits<double>::max();
    maxLat = std::numeric_limits<double>::lowest();
    minLon = std::numeric_limits<double>::max();
    maxLon = std::numeric_limits<double>::lowest();
}

const std::unordered_map<int, Node>& Graph::getNodes() const
{
    return nodes;
}
const std::unordered_map<int, std::vector<Edge>>& Graph::getAdjacencyList() const
{
    return adjacencyList;
}
double Graph::getMinLat() const
{
    return minLat;
}
double Graph::getMaxLat() const {
    return maxLat;
}
double Graph::getMinLon() const
{
    return minLon;
}
double Graph::getMaxLon() const
{
    return maxLon;
}

void Graph::loadFromXML(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QXmlStreamReader xml(&file);

    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartElement) {
            if (xml.name() == QString("node")) {
                int id = xml.attributes().value("id").toInt();
                double lat = xml.attributes().value("latitude").toDouble();
                double lon = xml.attributes().value("longitude").toDouble();

                nodes[id] = Node(id, lat, lon);

                if (lat < minLat) minLat = lat;
                if (lat > maxLat) maxLat = lat;
                if (lon < minLon) minLon = lon;
                if (lon > maxLon) maxLon = lon;
            }
            else if (xml.name() == QString("arc")) {
                int from = xml.attributes().value("from").toInt();
                int to = xml.attributes().value("to").toInt();
                double length = xml.attributes().value("length").toDouble();

                adjacencyList[from].emplace_back(to, length);
            }
        }
    }
    std::vector<Node> nodeList;
    nodeList.reserve(nodes.size());
    for(auto const& [id, n] : nodes) {
        nodeList.push_back(n);
    }
    kdTree.build(nodeList);
}

int Graph::getClosestNode(double x, double y, double width, double height)
{
    double widthGeo = maxLon - minLon;
    double heightGeo = maxLat - minLat;

    double targetLon = (x / width) * widthGeo + minLon;
    double targetLat = maxLat - (y / height) * heightGeo;

    return kdTree.findNearest(targetLat, targetLon);
}

std::vector<int> Graph::dijkstra(int startId, int endId)
{

    std::unordered_map<int, double> dist;
    std::unordered_map<int, int> parent;

    dist[startId] = 0;

    using PII = std::pair<double, int>;
    std::priority_queue<PII, std::vector<PII>, std::greater<PII>> pq;
    pq.push({0, startId});

    while (!pq.empty()) {
        int u = pq.top().second;
        double d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;
        if (u == endId) break;

        for (const auto& edge : adjacencyList[u]) {
            int v = edge.getToNodeId();
            double weight = edge.getLength();

            double current_dist_v = dist.count(v) ? dist[v] : std::numeric_limits<double>::max();
            if (dist[u] + weight < current_dist_v) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    std::vector<int> path;
    if (dist[endId] == std::numeric_limits<double>::max()) return path;

    int current = endId;
    while (current != startId) {
        path.push_back(current);
        current = parent[current];
    }
    path.push_back(startId);
    return path;
}
