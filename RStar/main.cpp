#include <iostream>

#include "rtree.hpp"

using namespace std;

main(){
    int from = 0;
    int to = 10;
    int amount = 10;
    int dimension = 2;
    int max_Children = 4;
    int min_Children = max_Children/2;
    
    RTree rtree(max_Children);
    rtree.init(from, to, amount, dimension);

}


