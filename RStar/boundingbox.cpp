#include <vector>
#include <iostream>
#include "boundingbox.hpp"

using namespace std;

BoundingBox::BoundingBox(){}

BoundingBox::BoundingBox(vector<float>& Min,  vector<float>& Max) : min(Min), max(Max){}