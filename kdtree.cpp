#include "kdtree.h"
#include <algorithm>
#include <cmath>
#include <limits>

KDTree::KDTree() : root(nullptr) {}

KDTree::~KDTree() {
    clearRecursive(root);
}

void KDTree::clearRecursive(KDNode* node) {
    if (!node) return;
    clearRecursive(node->left);
    clearRecursive(node->right);
    delete node;
}

void KDTree::build(std::vector<Node>& nodes) {
    root = buildRecursive(nodes, 0);
}

KDNode* KDTree::buildRecursive(std::vector<Node>& nodes, int depth) {
    if (nodes.empty()) return nullptr;

    int axis = depth % 2;
    int mid = nodes.size() / 2;

    auto comparator = [axis](const Node& a, const Node& b) {
        if (axis == 0) return a.getLat() < b.getLat();
        return a.getLon() < b.getLon();
    };

    std::nth_element(nodes.begin(), nodes.begin() + mid, nodes.end(), comparator);

    KDNode* node = new KDNode(nodes[mid]);

    std::vector<Node> leftNodes(nodes.begin(), nodes.begin() + mid);
    std::vector<Node> rightNodes(nodes.begin() + mid + 1, nodes.end());

    node->left = buildRecursive(leftNodes, depth + 1);
    node->right = buildRecursive(rightNodes, depth + 1);

    return node;
}

double KDTree::distance(const Node& a, const Node& b) {
    return std::sqrt(std::pow(a.getLat() - b.getLat(), 2) +
                     std::pow(a.getLon() - b.getLon(), 2));
}

int KDTree::findNearest(double lat, double lon) {
    if (!root) return -1;

    Node target(0, lat, lon);
    Node best = root->data;
    double minDst = std::numeric_limits<double>::max();

    searchRecursive(root, target, 0, best, minDst);
    return best.getId();
}

void KDTree::searchRecursive(KDNode* node, const Node& target, int depth, Node& best, double& minDst) {
    if (!node) return;

    double d = distance(node->data, target);
    if (d < minDst) {
        minDst = d;
        best = node->data;
    }

    int axis = depth % 2;
    double diff = (axis == 0) ? (target.getLat() - node->data.getLat()) : (target.getLon() - node->data.getLon());

    KDNode* near = (diff < 0) ? node->left : node->right;
    KDNode* far = (diff < 0) ? node->right : node->left;

    searchRecursive(near, target, depth + 1, best, minDst);

    if (std::abs(diff) < minDst) {
        searchRecursive(far, target, depth + 1, best, minDst);
    }
}
