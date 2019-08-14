#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>
#include "node.hpp"

using namespace std;

int Node::ID;

// evtl is Leaf als Parameter

// make an empty root (Node 4)
Node::Node() { 
    id = ID++;
    nodetype = 0;
    prefixLen = 0;
 }


// NODE constructor
Node::Node(vector<uint8_t> &keyvalues, vector<struct Node *> &childNodevalues) : prefixLen(0) { 
    id = ID++; 
    makeNode(keyvalues, childNodevalues);
}

void Node::makeNode(vector<uint8_t> &keyvalues, vector<struct Node *> &childNodevalues){
    if(keyvalues.size() <= 4) {
        node4(keyvalues, childNodevalues);
    } else if (keyvalues.size() <= 16) {
        node16(keyvalues, childNodevalues);
    } else if (keyvalues.size() <= 48) {
        node48(keyvalues, childNodevalues);
    } else if (keyvalues.size() <= 256){
        node256(keyvalues, childNodevalues);
    }
}


// make NODE 4 
void Node::node4(vector<uint8_t> &keyvalues, vector<struct Node *> &childNodevalues){
    nodetype = 0;
    // order keys and childnodes
    orderElems(&keyvalues, &childNodevalues);
    // assign keys and childnodes
    keys = keyvalues;
    childNodes = childNodevalues;
}

// make NODE 16
void Node::node16(vector<uint8_t> &keyvalues, vector<struct Node *> &childNodevalues){
    nodetype = 1;
    // order keys and childnodes
    orderElems(&keyvalues, &childNodevalues);
    // assign keys and childnodes
    keys = keyvalues;
    childNodes = childNodevalues;
}

// make NODE 48
void Node::node48(vector<uint8_t> &keyvalues, vector<struct Node *> &childNodevalues){
    nodetype = 2;
    // Problem, dass ints 2 bytes, nicht 1 byte(paper) lang sind
    // indirection from keys to childNode vector
    vector<uint8_t> tmp(256);
    int counter = 1;
    for(int i = 0; i < keyvalues.size(); i++){
        // append child pointers
        // copy vector?
        childNodes.push_back(childNodevalues[i]);
        // insert indirection to childpointers in keys vector
        tmp[keyvalues[i]] = counter; 
        counter++;
    }
    keys48 = tmp;
}


void Node::node256(vector<uint8_t> &keyvalues, vector<struct Node *> &childNodevalues){
    nodetype = 3;
    vector<struct Node *> tmp(256);
    int counter = 1;
    for(int i = 0; i < keyvalues.size(); i++){
        // insert childpointers into keys vector
        tmp[keyvalues[i]] = childNodevalues[i]; 
        counter++;
    }
    keys256 = tmp;
}


// gets a full node, makes it grow one size larger
void Node::overflow(){
    // NODE 4 overflows to NODE 16
    if(nodetype == 0) {
        vector<uint8_t> keytmp = keys;
        vector<struct Node *> childtmp = childNodes;
        for(int i = 0; i < keys.size(); i++)
            keys.pop_back();
        for(int i = 0; i < childNodes.size(); i++)
            childNodes.pop_back();
        node16(keytmp, childtmp);
    // NODE 16 overflows to NODE 48
    } else if (nodetype == 1) {
        vector<uint8_t> keytmp = keys;
        vector<struct Node *> childtmp = childNodes;
        for(auto &i : keys)
            keys.pop_back();
        for(auto &i : childNodes)
            childNodes.pop_back();
            node48(keytmp, childtmp);
    // NODE 48 overflows to NODE 256
    } else if (nodetype == 2) {
        vector<uint8_t> keytmp;
        for(int i = 0; i < keys48.size(); i++){
            if(keys48[i] > 0)
                keytmp.push_back(i);
        }
        vector<struct Node *> childtmp = childNodes;
        vector<uint8_t> tmp(256);
        keys48 = tmp;
        for(auto &i : childNodes)
            childNodes.pop_back();
        node256(keytmp, childtmp);
    } 
}


// evtl mit SIMD instruction????
vector<uint8_t> Node::getKeys(){
    if(nodetype <= 1) {
        return keys;
    } else if (nodetype == 2) {
        vector<uint8_t> tmp;
        for(int i = 0; i < keys48.size(); i++)
            if(keys48[i] != 0)
                tmp.push_back(i);
        return tmp;
    } else if (nodetype == 3){
        vector<uint8_t> tmp;
        for(int i = 0; i < keys256.size(); i++)
            if(keys256.size() != 0)
                tmp.push_back(i);
        return tmp;
    }

} 

struct Node * Node::setLastBit0(struct Node * ptr) {
    int64_t x = (int64_t) ptr;
    return (struct Node *) (x-1);
}

// get the value of a leaf back that was saved into an adress
/*int Node::getValue(struct Node &leaf){
    cout << " get value : " << &leaf;
    int64_t x =  ((int64_t) (&leaf))>>1;
    int64_t * tmp = (int64_t*) x;
     return (int) *tmp;
}*/
int Node::getValue(){
  //  cout << " get value : " << this;
    int64_t x =  ((int64_t) (this))>>1;
    int64_t * tmp = (int64_t*) x;
     return (int) *tmp;
}

// print a node
void Node::printNode(){
    cout << endl << "PRINT NODE: " << endl;
    // if the node is a leaf -> only print the adress that contains the value
    if((((int64_t) this) & 1) == 1) {
        int val = getValue();
        cout << " value:\t" << val << endl;
    // else print the node
    } else {
    cout << "nodetype: " << nodetype << endl;
    if(nodetype <=1) {
        cout << "keys: " << endl;
        for(auto &i : keys){
            cout << (int) i << "\t";
        }
        cout << endl << "childNodes: " << endl;
        for(auto &i : childNodes){
            cout << i << "\t";
        }
        cout << endl;
    } else if (nodetype == 2) {
        cout << "keys: " << endl;
        for(auto &i : keys48){
            cout << (int) i << "\t";
        }
        cout << endl << "childNodes: " << endl;
        for(auto &i : childNodes){
            cout << i << "\t";
        }
        cout << endl;
    } else if (nodetype == 3){
        cout << "keys: " << endl;
        for(auto &i : keys256){
            cout << i << "\t";
        }
    }
    }
}

vector<struct Node *> Node::getChildNodes(){
    if(nodetype <= 2) 
        return childNodes;
    vector<struct Node *> childtmp = childNodes;
    if(nodetype == 3){
        for(auto &i : keys256){
            if(i != 0)
                childtmp.push_back(i);
        }
    }
    return childtmp;
}

bool Node::isFull(){
    if(nodetype == 0 && keys.size() >= 4 || nodetype == 1 && keys.size() >= 16
        || nodetype == 2 && getKeys().size() >= 48 || nodetype == 3 && getKeys().size() >= 256) {
            return true;
    } else
        return false;
}


void Node::addChild(uint8_t keyelem, struct Node * node){
    // Node 4 and 16
    cout << "keyelem = " << (int) keyelem << " node: " << node << endl;
    node->getValue();
    if(nodetype <= 1) {
        // insert the key and the child pointer at the end, sort keys and childNodes according to the keys
        keys.push_back(keyelem);
        childNodes.push_back(node);
        auto p = sort_permutation(keys, [](const uint8_t b1, const uint8_t b2){
            return b1 < b2;
        });
        keys = apply_permutation(keys, p);
        childNodes = apply_permutation(childNodes, p);
        // Node 48
    } else if (nodetype == 2) {
        // append childNode
        childNodes.push_back(node);
        // insert index into the correct position in the key vector
        keys48[keyelem] = childNodes.size();
        // Node 256
    } else if (nodetype == 3){
        // insert child pointer into the correct position in the key vector
        keys256[keyelem] = node;
    } 
}



/* 
template nodes:
if(keys.size() <= 4) {
    } else if (keys.size() <= 16) {
    } else if (keys.size() <= 48) {
    } else if (keys.size() <= 256){
    } 
    
    if(nodetype == 0) {
    } else if (nodetype == 1 {
    } else if (nodetype == 2) {
    } else if (nodetype == 3){
    } */


void Node::orderElems(vector<uint8_t> * keys, vector<struct Node *> * childNodes ){
    auto p = sort_permutation(*keys, [](const uint8_t b1, const uint8_t b2){
            return b1 < b2;
        });
        // sort the entries as well as the data/childNode the same way
        *keys = apply_permutation(*keys, p);
        *childNodes = apply_permutation(*childNodes, p);
}

template <typename T, typename Compare> std::vector<std::size_t> Node::sort_permutation(const std::vector<T>& vec,
    Compare compare) {
    std::vector<std::size_t> p(vec.size());
    std::iota(p.begin(), p.end(), 0);
    std::sort(p.begin(), p.end(),
        [&](std::size_t i, std::size_t j){ return compare(vec[i], vec[j]); });
    return p;
}

template <typename T> void Node::apply_permutation_in_place(std::vector<T>& vec, const std::vector<std::size_t>& p) {
    std::vector<bool> done(vec.size());
    for (std::size_t i = 0; i < vec.size(); ++i){
        if (done[i]){
            continue;
        }
        done[i] = true;
        std::size_t prev_j = i;
        std::size_t j = p[i];
        while (i != j){
            std::swap(vec[prev_j], vec[j]);
            done[j] = true;
            prev_j = j;
            j = p[j];
        }
    }
}

template <typename T> std::vector<T> Node::apply_permutation(const std::vector<T>& vec, const std::vector<std::size_t>& p) {
    std::vector<T> sorted_vec(vec.size());
    std::transform(p.begin(), p.end(), sorted_vec.begin(),
        [&](std::size_t i){ return vec[i]; });
    return sorted_vec;
}

