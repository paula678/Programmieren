#ifndef __ENTRY__
#define __ENTRY__

#include <vector>
#include <iostream>
#include "node.hpp"

using namespace std;


class BoundingBox{

public:

    // Vectors with the min/max Point of the Node for each dimension
    vector<float> min;
    vector<float> max;

    //assert that mins are <= maxs
  
    // Constructors
    BoundingBox();

    BoundingBox(vector<float>& Min,  vector<float>& Max);
    
};

#endif