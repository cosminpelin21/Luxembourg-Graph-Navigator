#ifndef KDTREE_H
#define KDTREE_H

#include "kdnode.h"
#include <vector>

class KDTree
{
public:
    KDTree();
    ~KDTree();
    void build(std::vector<Node>& nodes);
    int findNearest(double lat, double lon);

private:
    KDNode* root;

    void clearRecursive(KDNode* node);
    KDNode* buildRecursive(std::vector<Node>& nodes, int depth);
    void searchRecursive(KDNode* node, const Node& target, int depth, Node& best, double& minDst);
    double distance(const Node& a, const Node& b);
};

#endif // KDTREE_H
