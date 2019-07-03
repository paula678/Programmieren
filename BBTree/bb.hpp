#ifndef _bb_hpp_
#define _bb_hpp_

#include <vector>

using namespace std;

class BB {

    public:
    // maximal amount of data objects a Leaf can hold
    int bmax;
    float isSuper;

    // BB is a super node
    int delimiterDim;
    vector<float> delimiterValues;
    vector<struct BB> buckets;
    
    // regular BB -> vector with elements
    vector<vector<float> > elements;
    vector<vector<float> * > tids;

    BB(int bmax);
    //create regular BB
    BB(int bmax, vector<vector<float> > elements, vector<vector<float> * > tids);

    // functions
    int amountRegularEntries();
    int amountSuperEntries();
    int chooseSuperChild(vector<float> &element, int k);
    void printRegular();
    void printSuper();


};

#endif