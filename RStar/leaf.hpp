#ifndef __RTREE_POINT_H__
#define __RTREE_POINT_H__

#include <vector>
#include <iostream>

using namespace std;


class Leaf{

public:

    // Vectors with the min/max Point of the Leaf for each dimension
    vector<float> min;
    vector<float> max;

    // Constructors
    Leaf() = default;
    Leaf(vector<float>& Min,  vector<float>& Max);

};

#endif