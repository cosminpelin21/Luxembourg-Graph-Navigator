#ifndef NODE_H
#define NODE_H

class Node {
private:
    int id;
    double lat;
    double lon;

public:
    Node();
    Node(int id, double lat, double lon);
    int getId() const;
    double getLat() const;
    double getLon() const;
};

#endif
