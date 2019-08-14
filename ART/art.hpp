#ifndef _art_hpp_
#define _art_hpp_

#include <vector>
#include "art.hpp"


using namespace std;

class ART {

    public:
    int key_complete;

    ART();

   // template <typename T> void x(const T * node);
   void x(int ** node);

    // Last Bit of Leaf Adress = 1
    // Last Bit of Node Pointer = 0

    // methods
    struct Node ** findChild(struct Node &node, uint8_t byte);
    struct Node * replace(struct Node &node, struct Node * leaf);
    void insert(struct Node ** nodepointer, vector<uint8_t> key, int64_t * leafvalue, int depth);
    void printTree(struct Node* tree, int depth);

    // helper methods
    vector<uint8_t> loadKey(struct Node &node);
    int64_t getLastBit(struct Node * ptr);
    struct Node * setLastBit1(struct Node * ptr);
    struct Node * setLastBit0(struct Node * ptr);
    void insert_sorted(std::vector<int> & vec, int item); 
    int getByte(int64_t integ, int x);
    vector<uint8_t> makeKeyvector(int64_t key);
    int getValue(struct Node &leaf);
    struct Node * saveLeaf(void * leaf);
    int checkPrefix(struct Node * node, vector<uint8_t> key, int depth);
    struct Node * search(struct Node * node, int key, int depth);

};

#endif