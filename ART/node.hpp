#ifndef __NODE__
#define __NODE__

#include <vector>
#include <iostream>

using namespace std;


class Node{

public:
    int id;

    // is usually size one, only gets larger with lazy expansion
    vector<uint8_t> prefix;
    int prefixLen;

    // Node 4 -> 0, Node 16 -> 1, Node 48 -> 2, Node 256 -> 3
    int nodetype;

    // give each node an ID
    static int ID;

    // father entry
 //   struct Node * father;

    // NODE 4 and 16
    // Vector with keys
    vector<uint8_t> keys;

    // NODE 48
    // vector with keys 48 -> indirection to childNodes vector
    // index do to indirection: i-1
    vector<uint8_t> keys48;

    // NODE 256
    // 256 -> direct storage of child pointers
    vector<struct Node *> keys256;


    // Vector with adresses to childnodes of the entries
    vector<struct Node *> childNodes; 

  
    // Constructors
    // root
    Node();
    // NODE
   Node(vector<uint8_t> &keys, vector<struct Node *> &childNodes);


   // methods
   void orderElems(vector<uint8_t> * keys, vector<struct Node *> * childNodes);

    // make nodes
    void makeNode(vector<uint8_t> &keyvalues, vector<struct Node *> &childNodevalues);
    // declare those private
    void node4(vector<uint8_t> &keyvalues, vector<struct Node *> &childNodevalues);
    void node16(vector<uint8_t> &keyvalues, vector<struct Node *> &childNodevalues);
    void node48(vector<uint8_t> &keyvalues, vector<struct Node *> &childNodevalues);
    void node256(vector<uint8_t> &keyvalues, vector<struct Node *> &childNodevalues);
    void overflow();
    void addChild(uint8_t keyelem, struct Node * node);
    vector<struct Node *> getChildNodes();

    // getter
    vector<uint8_t> getKeys();
    bool isFull();

   // print
   void printNode();


    //sort
    template <typename T, typename Compare> std::vector<std::size_t> sort_permutation(const std::vector<T>& vec,
    Compare compare);
    template <typename T> void apply_permutation_in_place(std::vector<T>& vec, const std::vector<std::size_t>& p);
    template <typename T> std::vector<T> apply_permutation(const std::vector<T>& vec, const std::vector<std::size_t>& p);


    // helper
    struct Node * setLastBit0(struct Node * ptr);
    int getValue();





};

#endif