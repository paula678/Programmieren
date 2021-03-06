
#ifndef _rtree_hpp_
#define _rtree_hpp_

#include <vector>
#include "node.hpp"
#include "boundingbox.hpp"

using namespace std;

class RTree {

    public: 

    int min_Children;
    int max_Children;
    int p;
    // every time a new root/level is created -> true, after every reinsert -> false
    // by this way reinsertion is only performed once on every level (first overflowTreatment)
    bool reins;

    // Constructor
    RTree(int max_children);

    // general Methods
    float randomFloat(float min, float max);

    template <typename T> std::vector<T> apply_permutation(const std::vector<T>& vec, const std::vector<std::size_t>& p);

    template <typename T> void apply_permutation_in_place(std::vector<T>& vec, const std::vector<std::size_t>& p);

    template <typename T, typename Compare> std::vector<std::size_t> sort_permutation(const std::vector<T>& vec,
    Compare compare);

    // print methods:
    void printTree(struct Node &node);

    void printTreeID(struct Node &node);

    void printBB(struct BoundingBox bb);

    // BoundingBox methods: -> in class BoundingBox?
    float marginValue(vector<struct BoundingBox> &entries);
    
    float rectangleArea(struct BoundingBox &entry);

    float overlapArea(struct BoundingBox &b1, struct BoundingBox &b2);
    
    bool fitsInNode(struct BoundingBox &elem, struct BoundingBox &entry);

    struct BoundingBox enclosingBB(vector<struct BoundingBox> &vec);
    
    bool overlap(struct BoundingBox &bb1, struct BoundingBox &bb2);
    
    struct BoundingBox newNode(struct BoundingBox &elem, struct BoundingBox &entry);

    float incrementationArea(struct BoundingBox &elem, struct BoundingBox &entry);

    float distanceBB(struct BoundingBox &b1, struct BoundingBox &b2);

    // Node methods:
    vector<int> minOverlap(struct Node &node, struct BoundingBox &elem);

    struct Node * chooseSubtree(struct Node &node, struct BoundingBox &elem);

    // lookup
    vector<struct BoundingBox *> findEntries(struct Node &node);

    struct Node * findLeaf(struct Node &node, struct BoundingBox * entry);

    // insert:
    vector<struct Node > split(struct Node &node);

    void reinsert(struct Node * &tree, struct Node &node);

    int overFlowTreatment(struct Node * node);

    void insert(struct Node &node, struct BoundingBox &entry);

    // create methods:
    struct Node createRTree(vector<struct BoundingBox> &elements);

    struct BoundingBox randomBB(float from, float to, int dimension);

    vector<struct BoundingBox> randomBBvector(float from, float to, int amount, int dimension);

    // tests:
    void test1(struct Node &root, struct BoundingBox &elem);

    void test2(struct Node &root, int from, int to, int amount, int dimension);

    // initial method that is called
    void init(int from, int to, int amount, int dimension);

};

#endif