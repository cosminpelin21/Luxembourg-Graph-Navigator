#ifndef EDGE_H
#define EDGE_H

class Edge {
private:
    int toNodeId;
    double length;

public:
    Edge();
    Edge(int toNodeId, double length);
    int getToNodeId() const;
    double getLength() const;
};

#endif
