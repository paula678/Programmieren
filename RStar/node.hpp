#ifndef __NODE__
#define __NODE__

#include <vector>
#include <iostream>
#include "boundingbox.hpp"

using namespace std;


class Node{

public:
    bool isLeaf;
    bool isRoot;
    int id;

    // give each node an ID
    static int ID;

    // father entry
    struct Node * father;

    // Vector with entries
    vector<struct BoundingBox> entries;

    // Vector with adresses to childnodes of the entries
    vector<struct Node *> childNode; 

    // Vector with actual data for leaf node
    vector<struct BoundingBox * > data;
  
    // Constructors
    // root
    Node();
    // Node
   Node(vector<struct BoundingBox> &entries, vector<struct Node *> &childNode);
    // Leaf
   Node(vector<struct BoundingBox> &entries, vector<struct BoundingBox *> &data);

};

#endif