#include <iostream>
#include <vector>
#include <random>
#include <bits/stdc++.h>
#include "rtree.hpp"


using namespace std;


RTree::RTree(int max_children) : max_Children(max_children){ min_Children = max_children * 0.5; }

//random Float erstellen
float RTree::randomFloat(float min, float max) {
    return (min + 1) + (((float) rand()) / (float) RAND_MAX) * (max - (min + 1)); 
}


// create random Bounding Boxes
vector<struct BoundingBox> RTree::createBB(int amount, float from, float to, int dimensions){
    vector<struct BoundingBox> data;
    for(int i = 0; i < amount; i++){
        // create a vector for the mins
        float fmin = randomFloat(from, to);
        vector<float> minVec(fmin, fmin);
        vector<float> maxVec(randomFloat(from, to), randomFloat(from, to));
        struct BoundingBox bb(minVec, maxVec);
        data.push_back(bb);
    }
    return data;
}


void RTree::printTree(struct Node &node, int counter){
    if(node.isLeaf)
        cout<< "leaf " << counter << ":\t"<< endl;
    else
        cout<< "node " << counter << ":\t"<< endl;
    for(auto &i : node.entries){
        // print min values for each node
        for(const auto &j : i.min){
            cout<< j << "\t";
        }
        cout<< endl;
        // print max values for each node
        for(const auto &j : i.max){
            cout<< j << "\t";
        }
        cout<< endl;
        if(!node.isLeaf)
            printTree(*i.childNode, counter++);
    }
}


// if a leaf fits in another node
// dimensions von child und node getrennt voneinander anschauen
bool RTree::fitsInNode(struct BoundingBox &elem, struct BoundingBox &entry){
    bool fits = true;
    for(int i = 0; i < entry.min.size(); i++){
        // check for each dimension if the child min is smaller than the node min -> child does not fit in node
        if(elem.min[i] < entry.min[i])
            fits = false;
        //equivalent for max
        if(elem.max[i] > entry.max[i])
            fits = false;

    }
    return fits;
}


// area of rectangle
float RTree::rectangleArea(struct BoundingBox &entry){
    float area = 1;
    // max absolute value - min abs for each dimension -> length of the edges; multiply for area of rectangle
    for(int i = 0; i < entry.min.size(); i++){
        float edge = fabs(entry.max[i] - entry.min[i]);
        area *= edge;
    }
    return area;
}


// make new rectangle out of two nodes
struct BoundingBox::BoundingBox RTree::newNode(struct BoundingBox &elem, struct BoundingBox &entry){
    //for each dimension determine smallest min and largest max of child and node
    vector<float> newMin (elem.min.size());
    vector<float> newMax(elem.min.size());
    for(int i = 0; i < elem.min.size(); i++){
        if(elem.min[i] < entry.min[i])
            newMin[i] = elem.min[i];
        else
            newMin[i] = entry.min[i];
        if(elem.max[i] > entry.max[i])
            newMax[i] = elem.max[i];
        else
            newMax[i] = entry.max[i];
    }
    /*
    cout<< "incrementation Area, the new mins are : " << endl;
    for(auto &i : newMin){
        cout<< i << endl;
    }
    cout<< "incrementation Area, the new maxs are : " << endl;
    for(auto &i : newMax){
        cout<< i << endl;
    }
    */

    // create new node with contains the child and the node to determine the new area
    return BoundingBox(newMin, newMax);
}


// calculate the incrementation of area for a node when inserting a child
float RTree::incrementationArea(struct BoundingBox &elem, struct BoundingBox &entry){
    float incrementation = 0;
    float entryArea = rectangleArea(entry);
    //create new node out of the child and the node
    struct BoundingBox n = newNode(elem, entry);
    float newArea = rectangleArea(n);
    // determine and return the incrementation of the area
    incrementation = newArea - entryArea;
    return incrementation;
}


// determine margin value of a vector of elements -> onl< for plane!!
float RTree::marginValue(vector<struct BoundingBox> &entries){
    float marginValue = 0;
    // determine bounding box of a set of rectangles -> new Bounding Box
    struct BoundingBox n = entries[0];
    for(auto &i : entries){
        n = newNode(i, n);
    }
    // determine margin -> each dimension
    for(int i = 0; i < entries[0].min.size(); i++){
       marginValue += 2 * fabs(n.max[i] - n.min[i]);
    }
    return marginValue;
}


struct BoundingBox RTree::enclosingBB(vector<struct BoundingBox> &vec){
    struct BoundingBox bb = newNode(vec[0], vec[1]);
    for(auto &i : vec)
        bb = newNode(bb, i);
    return bb;
}


// divide a set of m+1 entries into two groups
// determine axis, perpendicular to which the split is performed and the best distribution into two groups along that axis
// input : overloaded node with max_children + 1 elements
// return the two nodes that result from the split
vector<struct Node::Node> RTree::split(struct Node &node){
    int distributions = max_Children - (2 * min_Children) + 2;
    float minSum = __FLT_MAX__;
    int axis = 0;
    int sortStrategy = 0;
    int dist = 0;

    // for each axis
    for(int i = 0; i < node.entries[0].min.size(); i++){
        float minOverlapValue = __FLT_MAX__;
        float minAreaValue = __FLT_MAX__;
        int minDistAxis = 0;
        cout << "size tree min: " << node.entries[0].min.size() << endl;
        // for each axis compute sum of all margin-values of the different distributions
        int sumLower = 0;
        int sumUpper = 0;
        // sort entries by lower then by upper value of their rectangles
        vector<struct BoundingBox> sortLower = node.entries;
        sort(sortLower.begin(), sortLower.end(), [ i ](const struct BoundingBox& b1, const struct BoundingBox& b2){
            return b1.min[i] < b2.min[i];
        });
        vector<struct BoundingBox> sortUpper = node.entries;
        sort(sortUpper.begin(), sortUpper.end(), [ i ](const struct BoundingBox& b1, const struct BoundingBox& b2){
            return b1.max[i] < b2.max[i];
        });

    	// for each sort -> divide all M-2m+2 distributions into two groups
        for(int j = 0; j < distributions; j++){
            // if the split will be performed on this axis (min margin value for the axis): use minoverlap value for the split 
            // if tie -> min area value
            float minOverlapValueAxis = __FLT_MAX__;
            float minAreaValueAxis = __FLT_MAX__;
            int entries = min_Children - 1 + j;
            cout << "entries: " << entries;
            cout << " distribution " << j << "\t " << distributions << "\t" << endl;
            // first group contains (m-1)+k entries
            vector<struct BoundingBox> lowerGroup1(&sortLower[0],&sortLower[entries]);
            cout<< "lower group1 :\n"; 
           // printTree(lowerGroup1);
            vector<struct BoundingBox> upperGroup1(&sortUpper[0],&sortUpper[entries]);
            cout<< "upper group1 :\n"; 
           // printTree(upperGroup1);
            // second group contains remaining entries
            vector<struct BoundingBox> lowerGroup2(&sortLower[entries],&sortLower[sortLower.size()]);
            cout<< "lower group2 :\n"; 
           // printTree(lowerGroup2);
            vector<struct BoundingBox> upperGroup2(&sortUpper[entries],&sortUpper[sortUpper.size()]);
    
            // Compute sum of all margin-values of the different distributions
            sumLower += marginValue(lowerGroup1) + marginValue(lowerGroup2);
            cout << "margin lower: " << sumLower << " g1: " <<  marginValue(lowerGroup1) << " g2: " <<  marginValue(lowerGroup2);
            sumUpper +=  marginValue(upperGroup1) + marginValue(upperGroup2);
            cout << "margin upper: " << sumUpper;
            struct BoundingBox tmp1 = enclosingBB(sortLower);
            struct BoundingBox tmp2 = enclosingBB(sortUpper);

            // Determine the min overlap value and min area of the axis
                // lower distribution has min overlap value -> save dist
                if(overlapArea(tmp1, tmp2) < minOverlapValueAxis){
                    minOverlapValueAxis  = overlapArea(tmp1, tmp2);
                    minDistAxis = j;

                }
                // tie -> area value
                if(minOverlapValueAxis == overlapArea(tmp1, tmp2)){
                    if(rectangleArea(tmp1) < minAreaValueAxis){
                        minAreaValueAxis = minAreaValueAxis;
                        minDistAxis = j;
                    }

                }

             // choose axis with min sum as split axis, if found -> save relevant parameters to replicate this split on the actual tree
             cout << "min sum " << minSum << endl;
        }
        if(sumLower < sumUpper){
            if(sumLower < minSum){
                minSum = sumLower;
                axis = i;
                sortStrategy = 0;
                dist = minDistAxis;
                cout << "min sum lower " << minSum << endl;
            } else {
                if(sumUpper < minSum){
                    minSum = sumUpper;
                    axis = i;
                    sortStrategy = 1;
                    dist = minDistAxis;
                    cout << "min sum upper " << minSum << endl;
                }
            }
        }
    }


    // replicate the split with the saved parameters (sortStrategy, axis, dist -> which distribution to use)
    // sortStrategy 0 -> sort lower
    if(sortStrategy == 0){
        sort(node.entries.begin(), node.entries.end(), [ axis ](const struct BoundingBox& b1, const struct BoundingBox& b2){
            return b1.min[axis] < b2.min[axis];
        });
    // sortStrategy 1 -> sort upper
    } else {
        sort(node.entries.begin(), node.entries.end(), [ axis ](const struct BoundingBox& b1, const struct BoundingBox& b2){
            return b1.max[axis] < b2.max[axis];
        });
    }

    // divide all M-2m+2 distributions into two groups
    // !!! set fathernode to the old one
    int entries = min_Children - 1 + dist;
    vector<struct Node> group1(&node.entries[0], &node.entries[entries]);
    vector<struct Node> group2(&node.entries[entries], &node.entries[node.entries.size()]);
    
    //return the two new nodes
    return {group1, group2};
}


// overlap area of two Bounding Boxes
float RTree::overlapArea(struct BoundingBox &b1, struct BoundingBox &b2){
    float area = 1;
    // for each dimension
    if(overlap(b1, b2)){
        for(int i = 0; i < b1.min.size(); i++){
            float tmp = min(b1.max[i], b2.max[i]) - max(b1.min[i], b2.min[i]);
            area *= tmp;
        }
    } else
        area = 0;
    return area;
}


// check if two BB overlap ; only 2-dim!!!
bool RTree::overlap(struct BoundingBox &bb1, struct BoundingBox &bb2) {
    // BB don't overlap if one BB minimum is greater than the other BB maximum in some dim
    bool overlap = !(bb1.min[0] > bb2.max[0] || bb2.min[0] > bb1.max[0] || bb1.min[1] > bb2.max[1] || bb2.min[1] > bb1.max[1]);
    return overlap;
}


// minimum overlap enlargement of a node in the tree when inserting the elements -> several nodes if they have the same min overlap enlargement
vector<struct BoundingBox::BoundingBox *> RTree::minOverlap(struct Node &node, struct BoundingBox &elem){
    float minOverlapEnl = __FLT_MAX__;
    vector<struct BoundingBox *> minOverlapNode;
    // for each element in the current node of the tree determine the overlap enlargement
        for(int i = 0; i < node.entries.size(); i++){
     //   cout << node.entries[i].min[0] << node.entries[i].min[1] << endl;
        // determine current overlap area with the other elements
        float currentOverlap = 0;
        for(int j = 0; j < node.entries.size(); j++){
            if(&node.entries[j] != &node.entries[i]){
                currentOverlap += overlapArea(node.entries[i], node.entries[j]);
           //     cout << "overlaparea with " << node.entries[j].min[0] << node.entries[j].min[1] << " " << overlapArea(node.entries[i],node.entries[j]) << endl;
        }
        }
        // determine overlap area after inserting the element in the current element of the tree
        float newOverlap = 0;
        struct BoundingBox tmp = newNode(elem, node.entries[i]);
        for(int j = 0; j < node.entries.size(); j++){
            if(&node.entries[j] != &node.entries[i])
                newOverlap += overlapArea(tmp, node.entries[j]);
        }

      //  cout<< "newOverlap: " << newOverlap << " current " << currentOverlap << endl;
        // determine difference -> overlap enlargement
        // if the we find a new min overlap enlargement -> new vector with this subtree
        if((newOverlap - currentOverlap) < minOverlapEnl){
            minOverlapEnl = newOverlap - currentOverlap;
            minOverlapNode = {&node.entries[i]};
        // if we have a subtree with the same min overlap enlargement -> add the subtree to the vector
        } else if((newOverlap - currentOverlap) == minOverlapEnl)
            	minOverlapNode.push_back(&node.entries[i]);
    }
    return minOverlapNode;
}


// in which subtree to insert the rectangle for R*-tree -> recursively until a leaf to insert the element is found
struct Node::Node * RTree::chooseSubtree(struct Node &node, struct BoundingBox &elem, int level){
    struct BoundingBox * n;
    // if te tree is a leaf -> return it
    if(node.isLeaf)
        return &node;

    //  if children are leaves
    else if(node.entries[0].childNode->isLeaf){
        for(auto &j : node.entries){
        //    for(auto &i : j.childNode->entries)
         //       cout << i.min[0] << " " << i.min[1] << " " << i.max[0] << " " << i.max[1] << endl;
        }
        // choose entry in tree whose rectangle needs least overlap enlargements
        vector<struct BoundingBox *> minOverlapVector = minOverlap(node, elem);
     //   for(auto &i : minOverlapVector)
      //      cout << i->min[0] << " " << i->min[1] << " " << i->max[0] << " " << i->max[1] << endl;

        if(minOverlapVector.size() == 1)
            return minOverlapVector[0]->childNode;

        // if tie -> choose entry whose rectangle needs least incrementation of area
        float minIncrementation = __FLT_MAX__;
        vector<struct BoundingBox *> minAreaEnlargement;
        for(auto &i : minOverlapVector){
            float incr = incrementationArea(elem, *i);
            cout << incr << endl;
            // choose element with smallest incrementation of area
            if(incr < minIncrementation){
                minIncrementation = incr;
                minAreaEnlargement = {i};
            } else if (incr == minIncrementation)
                minAreaEnlargement.push_back(i);
        }
       // for(auto &i : minAreaEnlargement[0]->childNode->entries)
         //   cout << i.min[0] << " " << i.min[1] << " " << i.max[0] << " " << i.max[1] << endl;
        if(minAreaEnlargement.size() == 1)
            return minAreaEnlargement[0]->childNode;
        // if tie -> choose entry with smallest area
        float minArea = __FLT_MAX__;
        for(auto &i : minAreaEnlargement){
            float area = rectangleArea(*i);
            // choose element with smallest area
            if(area < minArea){
                minArea = area;
                n = i;
            }
        }
    // if children are not leaves
    } else {
        // choose entry in tree which needs least area enlargement
        float minIncrementation = __FLT_MAX__;
        for(auto &i : node.entries){
            float incr = incrementationArea(elem, i);
            // choose node with smallest incrementation of area
            if(incr < minIncrementation){
                minIncrementation = incr;
               n = &i;
            // if tie -> entry with smallest area
            } else if(incr == minIncrementation){
                if(rectangleArea(i) < rectangleArea(*n)){
                    minIncrementation = incr;
                    n = &i;
                }
            }
        }
    return n->childNode;
    }
}


// overflow treatment for split -> 0 or reinsertion -> 1
int RTree::overflowTreatment(vector<struct Node> &tree, struct Node &leaf, int level){

}

/*

// traverse recursively from root to leaf
void RTree::insert(vector<struct Node> &tree, struct Node &leaf){
    // if the tree is empty: create a new root
    if(tree.empty())
        tree = {createRoot(leaf)};
    // if the tree is a leaf: split when full, then insert leaf
    else {
        // find the node in which to place the entry
        vector<struct Node> * n = chooseSubtree(tree, leaf, 0);
        // if there is place in leaf n -> insert n the leaf in n
        if(n->size() < max_Children){
            n->push_back(leaf);
        }  else if(n->size() == max_Children){
            int treatment = overflowTreatment(tree, leaf, (*n)[0].level);
            // if split -> 0 was performed: propagate overflow treatment upwards if necessary
            // check father node !!!
            if(treatment == 0 ){

            // if split was performed on a root: create a new root
            } else if(tree[0].isRoot && treatment == 0){
                struct Node newRoot = newNode((*n)[0], (*n)[1]);
                newRoot.childNodes.push_back((*n)[0]);
                newRoot.childNodes.push_back((*n)[1]);
                newRoot.isRoot = true;
                tree = {newRoot};
            }

            // adjust all covering rectangles in the insertion path such that they are min. bounding
            // boxes enclosing their children rectangles
            
        }
        // else -> split the node
    }

}
*/

/*
struct Node RTree::createRTree(vector<struct BoundingBox> &elements){
    // no action when there are no leaves
    if(elements.empty())
        cout<< "Es sind keine Blätter zum Einfügen vorhanden" << endl;
    else {
        // create Root
        struct Node tree;
        // insert each leaf into the tree
        for(auto &i : elements){
            insert(tree, i);
        }
        printTree(tree, 0);
    }
}
*/

void RTree::init(int from, int to, int amount, int dimension){
    vector<float> min1 = {2,2};
    vector<float> max1 = {3,4};
    vector<float> min2 = {1,1};
    vector<float> max2 = {4,4};
    vector<float> min3 = {4,4};
    vector<float> max3 = {6,5};
    vector<float> min4 = {7,6};
    vector<float> max4 = {9,7};
    vector<float> min5 = {5,5};
    vector<float> max5 = {6,6};
    struct BoundingBox bb1(min1, max1);
    struct BoundingBox bb2(min2, max2);
    struct BoundingBox bb3(min3, max3);
    struct BoundingBox bb4(min4, max4);
    struct BoundingBox bb5(min5, max5);

    // make method createRandomData/rectangles
    vector<struct BoundingBox> data = {bb1, bb2, bb3, bb4, bb5};
  //  struct Node rtree = createRTree(data);
    struct Node rtree(data);
/*
    // test rectangle area
    float rec = rectangleArea(bb1);
    cout<< "the rectangle Area is: " << rec << endl;
*/
   //  test fits in other node
   // cout<<"test fits in node " << fitsInNode(bb1, bb2) << endl;
    // test incrementation of Area when inserting one nde into another one
   // cout<< "incrementation Area: " << incrementationArea(bb2, bb1) << endl;
    // test overlapArea
   // cout << "overlap Area: " << overlapArea(bb2, bb3) << endl;

  /*  //test createBB
    vector<struct BoundingBox> bb = createBB(4, 0, 11, 2);
    for(auto &i : bb){
        cout << i.min[0] << " ";
    }
    */
/*
   // test overlap
   cout << "overlap: " << overlap(bb1, bb2) << endl;
   cout << "overlap: " << overlap(bb1, bb3) << endl;
   */
    vector<float> minVec  = {4,6};
    vector<float> maxVec = {5,7};
    struct BoundingBox elem(minVec, maxVec);
    struct Node test(data);
 //   test.isLeaf = true;
    struct Node a(data);
    struct Node b(data);
    struct Node c(data);
    struct Node d(data);
    struct Node e(data);
    /*
    a.isLeaf = true;
    b.isLeaf = true;
    c.isLeaf = true;
    d.isLeaf = true;
    e.isLeaf = true;
    */
     test.entries[0].childNode = &a;
     test.entries[1].childNode = &b;
     test.entries[2].childNode = &c;
     test.entries[3].childNode = &d;
     test.entries[4].childNode = &e;
    test.entries[1].childNode->entries[0].max[0] = 4;
    test.entries[2].childNode->entries[0].max[0] = 5;
    test.entries[3].childNode->entries[0].max[0] = 6;
    test.entries[4].childNode->entries[0].max[0] = 7;
    test.entries[0].childNode->entries[0].max[0] = 8;
    
/*
    // test min overlap enlargement
    cout << "min overlap enlargement: " << endl;
    for(auto &i : minOverlap(test, elem))
        cout << i->min[0] << " " << i->min[1] << " " << i->max[0] << " " << i->max[1] << endl;
*/

// test choose subtree
    struct Node * subtree = chooseSubtree(test, elem, 0);
    cout << endl << endl;
    for(auto &i : subtree->entries)
        cout << i.min[0] << " " << i.min[1] << " " << i.max[0] << " " << i.max[1] << endl;

    // choose split axis
    /*
    vector<int> x = {1,2,3,4};
   for(auto &i : x)
        cout << i << " ";
    cout << "vector: ";
    vector<int> y(&x[0], &x[2]);
    for(auto &i : y)
        cout << i << " ";
        */
       
    vector<struct Node> n = split(test)[0];
    vector<struct Node> m = split(test)[1];
    
    cout << "choose split axis: " << endl<< endl << endl << endl;
    printTree(n);
    printTree(m);


}