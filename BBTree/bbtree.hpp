#ifndef _bbree_hpp_
#define _bbree_hpp_

#include <vector>
#include <queue>


#include "bb.hpp"

using namespace std;

class BBTree {

    public:
    // inner nodes split space into k subparts according to delimiter
    //dimension and k-1 delimiter values 
    int k;
    //max amount of data a regular leaf node can contain
    int bmax;

    BBTree(int k, int bmax);

    // helper methods
    float randomFloat(float min, float max);
    template<typename T> vector<vector<T>> splitVector(const vector<T>& vec, size_t n);
    template <typename T, typename Compare> vector<size_t> sort_permutation(const vector<T>& vec, Compare compare);
    template <typename T> void apply_permutation_in_place(vector<T>& vec, const vector<size_t>& p);
    template <typename T> vector<T> apply_permutation(const vector<T>& vec, const vector<size_t>& p);
    int heightTree(int ist);



    // BB vector functions
    int getDelimiterDim(vector<vector<float> > vec);
    void testGetDelimiterDim();
    int amountData(vector<BB> &bbvec);
    vector<int> cardinalityDimensions(vector<vector<float> > vec);


    // BB functions
    void intoSuper(struct BB &bb);

    // insert
    vector<float> BreadthFirstTravseral(vector<vector<float> > &sample, int buckets, vector<int> delimiters);
    vector<vector<float> > getBBtids(vector<struct BB> &bbvec);
    vector<vector<float> > getBBelements(vector<struct BB> &bbvec);
   // vector<int> delimiterValues(vector<vector<float> > sample, int buckets, vector<int> delimiters);
    void reorganizeIndex(vector<float> &ist, vector<int> &delimiters, vector<struct BB> &bbvec);
    int traverseIst(vector<float> &ist, vector<int> &delimiters, vector<struct BB> &bbvec, vector<float> &element);
    void insert(vector<float> &ist, vector<int> &delimiters, vector<struct BB> &bbvec, vector<float> &element);

    // search
    struct BB * findData(vector<float> &ist, vector<int> &delimiters, vector<struct BB> &bbvec, vector<float> &element);
    vector<int> completeRangeQueryIndexes(vector<float> &ist, vector<int> &delimiters, vector<struct BB> &bbvec, vector<float> &element1, vector<float> &element2, int level, int position);
    vector<vector<float> * > completeRangeQuery(vector<float> &ist, vector<int> &delimiters, vector<struct BB> &bbvec, vector<float> &element1, vector<float> &element2);
    vector<int> partialRangeQuery(vector<float> &ist, vector<int> &delimiters, vector<struct BB> &bbvec, vector<float> &element1, vector<float> &element2, int level, int position);

    // create functions 
    vector<vector<float> > createData(int from, int to, int dimensions, int amount);

    // print functions
    void printQueue(queue<vector<vector<float> > > &q);
    void printData(vector<vector<float> > &data);
    void printIst(vector<float> &ist, vector<int> &delimiters);
    void printTree(vector<float> &ist, vector<int> &delimiters, vector<BB> &bbvec);
    void printBBvec(vector<struct BB > bbvec);

    void printSuper(struct BB &super);

    void init(int from, int to, int dimensions, int amount);

};

#endif