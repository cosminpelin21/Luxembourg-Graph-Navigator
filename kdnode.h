#ifndef KDNODE_H
#define KDNODE_H

#include "node.h"

struct KDNode {
    Node data;
    KDNode* left;
    KDNode* right;

    KDNode(Node n) : data(n), left(nullptr), right(nullptr) {}
};

#endif // KDNODE_H
