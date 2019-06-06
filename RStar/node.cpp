#include <vector>
#include <iostream>
#include "node.hpp"

using namespace std;

int Node::ID;

// create empty root
Node::Node() : isRoot(true), isLeaf(true) { id = ID++; }

// create Node
Node::Node(vector<struct BoundingBox> &entries, vector<struct Node *> &childNode) : isLeaf(false), isRoot(false), entries(entries), childNode(childNode){ 
    id = ID++; 
    for(auto &i: childNode)
        i->father = this;
    }

//create Leaf that is no root
Node::Node(vector<struct BoundingBox> &entries, vector<struct BoundingBox *> &data) : isLeaf(true), isRoot(false), entries(entries), data(data){ id = ID++; }

// add father node if no root to each node