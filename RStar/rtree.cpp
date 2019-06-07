#include <iostream>
#include <vector>
#include <random>
#include <bits/stdc++.h>
#include "rtree.hpp"


using namespace std;


RTree::RTree(int max_children) : max_Children(max_children), reins(true) { 
    min_Children = max_children * 0.5; 
    p = 0.3 * max_Children;
}

//random Float erstellen
float RTree::randomFloat(float min, float max) {
    return (min + 1) + (((float) rand()) / (float) RAND_MAX) * (max - (min + 1)); 
}


// only print the ids of a tree
void RTree::printTreeID(struct Node &node){
    if(node.isLeaf)
        cout<< "leaf "<< ":\t" << node.id << endl;
    else
        cout<< "node " << ":\t" << node.id << endl;
        // print id for each node
    if(!node.isLeaf){
        for(auto &i : node.childNode)
            printTreeID(*i);
    }
}


void RTree::printTree(struct Node &node){
    if(node.isLeaf)
        cout<< "leaf " << node.id << ":\t"<< endl;
    else
        cout<< "node " << node.id << ":\t"<< endl;
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
    }
    if(!node.isLeaf){
    for(auto &i : node.childNode)
            printTree(*i);
    }
}


// if an elem fits in another entry
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


// determine the enclosing BoundingBox to a a set of BoundingBoxes
struct BoundingBox::BoundingBox RTree::enclosingBB(vector<struct BoundingBox> &vec){
    struct BoundingBox bb = vec[0];
    for(auto &i : vec)
        bb = newNode(bb, i);
    return bb;
}


// divide a set of m+1 entries into two groups
// determine axis, perpendicular to which the split is performed and the best distribution into two groups along that axis
// input : overloaded node with max_children + 1 elements
// return the two nodes that result from the split
vector<struct Node::Node> RTree::split(struct Node &node){
    cout<< "split node " << node.id << " with datasize " << node.data.size() << " and childNode size " << node.childNode.size() << endl; 
    int distributions = max_Children - (2 * min_Children) + 2;
    float minSum = __FLT_MAX__;
    int axis = 0;
    int sortStrategy = 0;

    // for each axis
    for(int i = 0; i < node.entries[0].min.size(); i++){
        float sum = 0;
 //       cout << "size tree min: " << node.entries[0].min.size() << endl;
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

            int entries = min_Children - 1 + j;
 //           cout << "entries: " << entries;
 //           cout << " distribution " << j << "\t " << distributions << "\t" << endl;
            // first group contains (m-1)+k entries
            vector<struct BoundingBox> lowerGroup1(&sortLower[0],&sortLower[entries]);
 //           cout<< "lower group1 :\n"; 
 //         for(auto &i : lowerGroup1)
 //               cout << i.min[0] << " " << i.min[1] << " " << i.max[0] << " " << i.max[1] << endl;
           // printTree(lowerGroup1);
            vector<struct BoundingBox> upperGroup1(&sortUpper[0],&sortUpper[entries]);
 //           cout<< "upper group1 :\n"; 
//            for(auto &i : upperGroup1)
//                cout << i.min[0] << " " << i.min[1] << " " << i.max[0] << " " << i.max[1] << endl;
           // printTree(upperGroup1);
            // second group contains remaining entries
            vector<struct BoundingBox> lowerGroup2(&sortLower[entries],&sortLower[sortLower.size()]);
//            cout<< "lower group2 :\n"; 
//            for(auto &i : lowerGroup2)
//                cout << i.min[0] << " " << i.min[1] << " " << i.max[0] << " " << i.max[1] << endl;
           // printTree(lowerGroup2);
            vector<struct BoundingBox> upperGroup2(&sortUpper[entries],&sortUpper[sortUpper.size()]);
 //           cout<< "upper group2 :\n"; 
 //           for(auto &i : upperGroup2)
 //               cout << i.min[0] << " " << i.min[1] << " " << i.max[0] << " " << i.max[1] << endl;
    
            // Compute sum of all margin-values of the different distributions
            sumLower = marginValue(lowerGroup1) + marginValue(lowerGroup2);
            sumUpper =  marginValue(upperGroup1) + marginValue(upperGroup2);
//            cout << " sum " << sum; 
            sum = sum +  marginValue(lowerGroup1) + marginValue(lowerGroup2) + marginValue(upperGroup1) + marginValue(upperGroup2);
 //           cout << "margin lower: " << sumLower << " margin upper: " << sumUpper << " sum " << sum;

        }

          // choose axis with min margin sum as split axis, if found
//            cout << "min sum " << minSum << endl;
            if(sum < minSum){
                minSum = sum;
                axis = i;
//                cout << "min sum ... " << minSum << " axis " << axis << endl;
            } else {
//                cout << "no sum ... " << sum << " axis " << i << endl;
            }
    }


    // determine the split distribution
        float minOverlapValue = __FLT_MAX__;
        float minAreaValue = __FLT_MAX__;
        int distribution = 0;

    // sort the the nodes according to the determined axis upwards and downwards
       vector<struct BoundingBox> sortLower = node.entries;
        sort(sortLower.begin(), sortLower.end(), [ axis ](const struct BoundingBox& b1, const struct BoundingBox& b2){
            return b1.min[axis] < b2.min[axis];
        });
        vector<struct BoundingBox> sortUpper = node.entries;
        sort(sortUpper.begin(), sortUpper.end(), [ axis ](const struct BoundingBox& b1, const struct BoundingBox& b2){
            return b1.max[axis] < b2.max[axis];
        });


    	// for each sort -> divide all M-2m+2 distributions into two groups
        for(int j = 0; j < distributions; j++)  {
            // if the split will be performed on this axis (min margin value for the axis): use minoverlap value for the split 
            // if tie -> min area value

 //       cout << "size tree min: " << node.entries[0].min.size() << endl;
        int sumLower = 0;
        int sumUpper = 0;
        // sort entries by lower then by upper value of their rectangles
    
            int entries = min_Children - 1 + j;
//            cout << "entries: " << entries;
//            cout << " distribution " << j << "\t " << distributions << "\t" << endl;
            // first group contains (m-1)+k entries
            vector<struct BoundingBox> lowerGroup1(&sortLower[0],&sortLower[entries]);
 /*           cout<< "lower group1 :\n"; 
            for(auto &i : lowerGroup1)
                cout << i.min[0] << " " << i.min[1] << " " << i.max[0] << " " << i.max[1] << endl;
 */           vector<struct BoundingBox> upperGroup1(&sortUpper[0],&sortUpper[entries]);
  /*          cout<< "upper group1 :\n"; 
            for(auto &i : upperGroup1)
                cout << i.min[0] << " " << i.min[1] << " " << i.max[0] << " " << i.max[1] << endl;
   */         // second group contains remaining entries
            vector<struct BoundingBox> lowerGroup2(&sortLower[entries],&sortLower[sortLower.size()]);
/*            cout<< "lower group2 :\n"; 
            for(auto &i : lowerGroup2)
                cout << i.min[0] << " " << i.min[1] << " " << i.max[0] << " " << i.max[1] << endl;
  */          vector<struct BoundingBox> upperGroup2(&sortUpper[entries],&sortUpper[sortUpper.size()]);
/*            cout<< "upper group2 :\n"; 
            for(auto &i : upperGroup2)
                cout << i.min[0] << " " << i.min[1] << " " << i.max[0] << " " << i.max[1] << endl;
*/
            // create bounding boxes out of the vectors to determine the area and overlap values of the distributions
            struct BoundingBox tmp1= enclosingBB(lowerGroup1);
            struct BoundingBox tmp2 = enclosingBB(lowerGroup2);
            struct BoundingBox tmp3 = enclosingBB(upperGroup1);
            struct BoundingBox tmp4 = enclosingBB(upperGroup2);

            // Determine the min overlap value and min area of the axis
 //           cout << "..." << tmp1.min[0] << " " << tmp1.min[1] << " " <<  tmp1.max[0] << " " << tmp1.max[1] << endl;
//            cout << "..." << tmp2.min[0] << " " << tmp2.min[1] << " " <<  tmp2.max[0] << " " << tmp2.max[1] << endl;
            float tmpOverlapLower = overlapArea(tmp1, tmp2);
            float tmpOverlapUpper = overlapArea(tmp3, tmp4);
            float tmpAreaLower = rectangleArea(tmp1) + rectangleArea(tmp2);
            float tmpAreaUpper = rectangleArea(tmp3) + rectangleArea(tmp4);
//            cout << "overlaplower " << tmpOverlapLower << " overlapupper " << tmpOverlapUpper << " tmparealower " << tmpAreaLower << " tmpAreaUpper " << tmpAreaUpper << endl;
//            cout  << "minoverlap " << minOverlapValue << " minarea " << minAreaValue << endl;

        //check if the current distribution has the min overlap value, if tie: min area vlue

            // for lower distribution
            if(tmpOverlapLower < minOverlapValue){
                minOverlapValue  = tmpOverlapLower;
                minAreaValue = tmpAreaLower;
                distribution = j;
                sortStrategy = 0;
                cout  << 1 << " minoverlap " << minOverlapValue << " minarea " << minAreaValue << endl;
            // tie -> area value
            } else if(tmpOverlapLower ==  minOverlapValue){
                if(tmpAreaLower < minAreaValue){
                    minAreaValue = tmpAreaLower;
                    distribution = j;
                    sortStrategy = 0;
                    cout  << 2 << " minoverlap " << minOverlapValue << " minarea " << minAreaValue << endl;
                }
            }

            // for upper distribution
            if(tmpOverlapUpper < minOverlapValue){
                minOverlapValue  = tmpOverlapUpper;
                minAreaValue = tmpAreaUpper;
                distribution = j;
                sortStrategy = 0;
                 cout  << 3 << " minoverlap " << minOverlapValue << " minarea " << minAreaValue << endl;
                // tie -> area value
            } else if(tmpOverlapUpper ==  minOverlapValue){
                if(tmpAreaUpper < minAreaValue){
                    minAreaValue = tmpAreaUpper;
                    distribution = j;
                    sortStrategy = 0;
                    cout  << 4 << " minoverlap " << minOverlapValue << " minarea " << minAreaValue << endl;
                }
                cout  << 5 << " minoverlap " << tmpAreaUpper << " minarea " << minAreaValue << endl;
            }
            cout  << j << " minoverlap.. " << minOverlapValue << " minarea " << minAreaValue << " minDistribution " << distribution << endl;
        }        
    cout << " axis " << axis<< " sort " << sortStrategy << " distribution " << distribution << endl;
 
    // replicate the split with the saved parameters (sortStrategy, axis, dist -> which distribution to use)
    cout << endl << "replicate the split" << endl;
/*    // sortStrategy 0 -> sort lower
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
 */   
    // sortStrategy 0 -> sort lower
    if(sortStrategy == 0){
        auto p = sort_permutation(node.entries, [ axis ](const struct BoundingBox& b1, const struct BoundingBox& b2){
            return b1.min[axis] < b2.min[axis];
        });
        // sort the entries as well as the data/childNode the same way
        node.entries = apply_permutation(node.entries, p);
        if(node.isLeaf)
            node.data = apply_permutation(node.data, p);
        else
            node.childNode = apply_permutation(node.childNode, p); 
    // sortStrategy 1 -> sort upper
    } else {
        auto p = sort_permutation(node.entries, [ axis ](const struct BoundingBox& b1, const struct BoundingBox& b2){
            return b1.max[axis] < b2.max[axis];
        });
        node.entries = apply_permutation(node.entries, p);
        if(node.isLeaf)
            node.data = apply_permutation(node.data, p);
        else
            node.childNode = apply_permutation(node.childNode, p);
    }
    cout << "nodes sorted with sortStrategy " << sortStrategy << endl;


    // divide all M-2m+2 distributions into two groups -> along the distribution determined before
    // !!! set fathernode to the old one
    int entries = min_Children - 1 + distribution;
//    cout << "entries " << entries << endl;
    vector<struct BoundingBox> group1(&node.entries[0], &node.entries[entries]);
    cout << "group 1 size " << group1.size();
    vector<struct BoundingBox> group2(&node.entries[entries], &node.entries[node.entries.size()]);
    cout << "group 2 size " << group2.size() << " datasize " << node.data.size() << endl;

    // if the node was a leaf -> split in two new leafs
    if(node.isLeaf){
        cout << "node is leaf" << endl;
        vector<struct BoundingBox *> data1(&node.data[0], &node.data[entries]);
        cout << "data 1 size " << data1.size();
        vector<struct BoundingBox *> data2(&node.data[entries], &node.data[node.data.size()]);
        cout << "data 2 size " << data2.size();
        struct Node node1(group1, data1);
        struct Node node2(group2, data2);
        cout<< "print the two nodes" << endl;
        printTreeID(node1);
        printTreeID(node2);
        //return the two new nodes
        return {node1, node2};
    // if the node was an inner node -> split in two new inner nodes
    } else{
        cout << "node is not leaf" << endl;
        vector<struct Node *> childNodes1(&node.childNode[0], &node.childNode[entries]);
        vector<struct Node *> childNodes2(&node.childNode[entries], &node.childNode[node.childNode.size()]);
        struct Node node1(group1, childNodes1);
        struct Node node2(group2, childNodes2);
        //return the two new nodes
        return {node1, node2};
    }
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
vector<int> RTree::minOverlap(struct Node &node, struct BoundingBox &elem){
    float minOverlapEnl = __FLT_MAX__;
    vector<struct BoundingBox *> minOverlapNode;
    vector<int> minOverlapNod;
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
           // minOverlapNode = {&node.entries[i]};
            minOverlapNod = {i};
        // if we have a subtree with the same min overlap enlargement -> add the subtree to the vector
        } else if((newOverlap - currentOverlap) == minOverlapEnl)
            	minOverlapNod.push_back(i);
    }
    return minOverlapNod;
}


// in which subtree to insert the rectangle for R*-tree -> recursively until a leaf to insert the element is found
struct Node::Node * RTree::chooseSubtree(struct Node &node, struct BoundingBox &elem){
    cout << "chooooooooooooose SUbtreeeeeeeeeeeee   " << node.isLeaf << endl;
    int n = 0;
 //   cout << "elem : " << elem.min[0] << " " << elem.min[1] << " " << elem.max[0] << " " << elem.max[1]  << endl;
    // if te tree is a leaf -> return it
    if(node.isLeaf)
        return &node;

    //  if children are leaves
    else if(node.childNode[0]->isLeaf){
        cout << "childNode bb: " << endl;
        int counter = 0;
 /*       for(auto &i : node.entries){
                cout << counter << " " << i.min[0] << " " << i.min[1] << " " << i.max[0] << " " << i.max[1] << " " << node.childNode[counter] << endl;
                counter++;
        }
 */       // choose entry in tree whose rectangle needs least overlap enlargements
        vector<int> minOverlapVector = minOverlap(node, elem);
/*        cout << "minoverlapvector :" << endl;
        for(auto &i : minOverlapVector)
            cout << i << " ";
            cout<< endl;
*/
        if(minOverlapVector.size() == 1)
            return node.childNode[minOverlapVector[0]];
        
        cout << "tie:" << endl;
        // if tie -> choose entry whose rectangle needs least incrementation of area
        float minIncrementation = __FLT_MAX__;
        vector<int> minAreaEnlargement;
        for(auto &i : minOverlapVector){
            float incr = incrementationArea(elem, node.entries[i]);
            cout << " incr " << incr << " minincrementation " << minIncrementation << endl;
            // choose element with smallest incrementation of area
            if(incr < minIncrementation){
                minIncrementation = incr;
                minAreaEnlargement = {i};
                cout << " incr smaller as minIncrementation" << endl;
            } else if (incr == minIncrementation)
                minAreaEnlargement.push_back(i);
                cout << "incr == minINcrementation" << endl ;
        }
        if(minAreaEnlargement.size() == 1)
            return node.childNode[minAreaEnlargement[0]];
        cout << " tie area enlargement " << endl;
        // if tie -> choose entry with smallest area
        float minArea = __FLT_MAX__;
        for(auto &i : minAreaEnlargement){
            float area = rectangleArea(node.entries[i]);
            cout << " area " << area << " minareaenlargement " << i << endl;
            // choose element with smallest area
            if(area < minArea){
                minArea = area;
                n = i;
                cout << "area smaller minarea " << endl;
            }
        }
    // if children are not leaves
    } else {
        cout << " children are not leaves " << endl;
        cout << "childNode bb: " << endl;
        int counter = 0;
        for(auto &i : node.entries){
                cout << counter << " " << i.min[0] << " " << i.min[1] << " " << i.max[0] << " " << i.max[1] << " " << node.childNode[counter] << endl;
                counter++;
        }
        // choose entry in tree which needs least area enlargement
        float minIncrementation = __FLT_MAX__;
        for(int i = 0; i < node.entries.size(); i++){
            float incr = incrementationArea(elem, node.entries[i]);
            cout << i <<  " incr " << incr << " minincrementation " << minIncrementation << endl;
            // choose node with smallest incrementation of area
            if(incr < minIncrementation){
                minIncrementation = incr;
               n = i;
               cout<< "incr smaller minINcrementation" << endl;
            // if tie -> entry with smallest area
            } else if(incr == minIncrementation){
                cout<< "incr == minINcrementation" << endl;
                if(rectangleArea(node.entries[i]) < rectangleArea(node.entries[n])){
                    minIncrementation = incr;
                    n = i;
                    cout << "rectangle smaller previous rectangle " << endl;
                }
            }
        }
        return chooseSubtree(*node.childNode[n], elem);
    }
    return node.childNode[n];
}


// determine the distance between the centres of two BouningBoxes
float RTree::distanceBB(struct BoundingBox &b1, struct BoundingBox &b2){
    float distance = 0;
    
    for(int i = 0; i < b1.min.size(); i++){
        float p = b1.min[i] + 0.5 * (b1.max[i] - b1.min[i]);
        float q = b2.min[i] + 0.5 * (b2.max[i] - b2.min[i]);

        distance += powf(q - p,2);
    }
    distance = sqrtf(distance);
}


// find the Leaf of an entry
struct Node::Node * RTree::findLeaf(struct Node &node, struct BoundingBox * entry){
    if(!node.isLeaf){
        struct Node * sol;
        cout<< "is not a leaf "<< endl;
        for(int i = 0; i < node.entries.size(); i++){
            if(fitsInNode(*entry, node.entries[i])){
                cout << "fits in node " << node.id << endl;
                sol = findLeaf(*node.childNode[i], entry);
                if(sol != 0)
                    return sol;
            }
        }
        return 0;
    } else {
        cout<< "is a leaf " << node.data.size() << entry << endl;
      for(auto &i : node.data){
            cout << " " << i << endl;
            if(i == entry)
                return &node;
        }
       cout <<" no leaf matchess" << endl;
        return 0;
    }
}

/*
// change for lookup method!!
// find the entries in the Leaf of one Node
vector<struct BoundingBox::BoundingBox *> RTree::findEntries(struct Node &node){
    vector<struct BoundingBox *> entries;
    if(node.isLeaf)
        return node.data;
    for(auto &i : node.childNode){
        vector<struct BoundingBox *> tmp = findEntries(*i);
        entries.insert(entries.end(), tmp.begin(), tmp.end());
    } 
    return entries;
}
*/

// reinsert the first entries of a node
void RTree::reinsert(struct Node * &tree, struct Node &node){
    float mindDistance = __FLT_MAX__;
    // BoundingBox of the node
    struct BoundingBox n = enclosingBB(node.entries);
    cout << "encloseing BondingBox of  the node " << node.id << endl;

    // vector for the distances of the min_children + 1 entries to the centre of the BoundingBox of the node
    vector<float> distances;

    // for all entries distence between centre of tne entry and the BoundingBox of the node
    for(auto &i : node.entries)
        distances.push_back(distanceBB(i, n));
    cout << "distances: ";
    for(auto &i: distances)
        cout << i;
    cout << endl;

    // sort the distances by decreasing order
    sort(distances.begin(), distances.end(), greater<float>());

    // remove the first p entries from the node
    vector<struct BoundingBox *> deletedData;
    // remove the entries
    cout << endl << "REINSERTION:  p: " << p << endl; 
    for(int i = 0; i < p; i++){
        deletedData.push_back(node.data[i]);
    }  
    for(auto &i: deletedData){
        // erase the first element !!!
        node.entries.erase(node.entries.begin());   
        node.data.erase(node.data.begin());  
        insert(*tree, *i);
    }
}


// gets as input a node with max_children+1 entrie -> decides if to call split or reinsert
int RTree::overFlowTreatment(struct Node * node){
    	// call reinsert
        if(!node->isRoot && reins){
            reins = false;
            cout<< "node is not the root and has not overflown before" << endl;
            // a reinsertion shall only be performed once on a node -> set overflow of the node to true
            struct Node * root = node;
            while(!root->isRoot){
                root = root->father;
            }
            cout << endl << endl << endl << "CALL REINSERT" << endl;
            reinsert(root, *node);
            return 0;

        // call split
        } else {
            struct Node * spl0;
            struct Node * spl1;
            vector<struct Node> spl = split(*node);
            if(node->isLeaf){
                cout << "perform split leaf: " << endl;
                spl0  = new Node(spl[0].entries, spl[0].data);
           //     node->entries = spl[0].entries;
           //     node->data = spl[0].data;
                spl1  = new Node(spl[1].entries, spl[1].data);
            } else {
                cout << "perform split not leaf:" << endl;
                spl0  = new Node(spl[0].entries, spl[0].childNode);
            //    node->entries = spl[0].entries;
             //   node->childNode = spl[0].childNode;
                spl1  = new Node(spl[1].entries, spl[1].childNode);
            }

            cout<< "solution split:" << endl;
            printTreeID(*spl0);
            printTreeID(*spl1);
            if(!node->isRoot){
                // split the old node -> create the two new BoundingBoxes, append to the entries of the fathernode
               // node->entries = {enclosingBB(spl0->entries), enclosingBB(spl1->entries)};
 //               node->father->entries.push_back(enclosingBB(node->entries));

 // -> Propagate enclosing rectangle upwards!!!!!
                cout << endl << endl << endl << "SPL! ISLEEEEEEEEEEEEEEEAF " << spl1->isLeaf  << " " << node->isLeaf << endl;
                node->father->entries.push_back(enclosingBB(spl1->entries));
                // create the two new child nodes from the split -> set the fathernodes if the node is not the root
 //               spl0->father = node->father;
                spl1->father = node->father;
            //    node->childNode = {spl0, spl1};
 //               node->father->childNode.push_back(spl0);
                node->father->childNode.push_back(spl1);
                node->entries = spl0->entries;
                node->childNode = spl0->childNode;
                node->data = spl0->data;
                for(auto &i: node->childNode)
                    i->father = node;
                delete spl0;
                cout<< "tree at end of overFlowTreatment " << endl;
            } else{
                // old root becomes the new one
                reins = true;
                node->entries = {enclosingBB(spl0->entries), enclosingBB(spl1->entries)};
                spl0->father = node;
                spl1->father = node;
                node->childNode = {spl0, spl1};
        	    node->isLeaf = false; 
            
 //               vector<struct BoundingBox> entr = {enclosingBB(node->entries), enclosingBB(spl1->entries)};
 //               vector<struct Node *> children = {node, spl1};
 //               struct Node * root = new Node(entr, children);
                cout<< "tree at end of overFlowTreatment " << endl;
                printTreeID(*node);
                printTree(*node);
            }
            return 1;
        }
}


// traverse recursively from root to leaf
void RTree::insert(struct Node &node, struct BoundingBox &entry){
    // find the node in which to place the entry
    struct Node * n = chooseSubtree(node, entry);
    cout << " subtree choseeeeeeeeeeeeeeeeeeeeeen\t" << n->id << "\t" << n->isLeaf << endl;
    // insert n the leaf in n
    n->entries.push_back(entry);
    n->data.push_back(&entry);
    // if there are max_children+1 entries
    if(n->entries.size() > max_Children){
        cout << "overflow of the node " << n->id << endl;
        int overflow = overFlowTreatment(n);
  /*      cout<< "after overflowTreatment " << endl;
        printTreeID(node);
        printTree(node);
*/        cout << "overflow treatment is " << overflow << endl;
        if(overflow = 1){
            // if split was performed: propagate overflow treatment upwards if necessary
            struct Node * tmp = n;
            while(!tmp->isRoot && tmp->father->entries.size() > max_Children){
                cout<< "propagate overflow treatment upwards for fathernode " << tmp->father->id << endl;
                overFlowTreatment(tmp->father);
                tmp = tmp->father;
            }
        }
    }
    // adjust all covering rectangles in the insertion path such that they are min. bounding
    // boxes enclosing their children rectangles
    struct Node * tmp = n;
    while(!tmp->isRoot){
        tmp = tmp->father;
        cout<< "ADJUST BBBBBBBBBBBBBBBBBBBS" << tmp->id <<  endl;
        for(int i = 0; i < tmp->entries.size(); i++){
            tmp->entries[i] = enclosingBB(tmp->childNode[i]->entries);
        }
    }
    cout<< endl << endl << endl << "TREE AT END OF INSERT" << endl;
    printTreeID(node);
    printTree(node);

}


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


// generate a random BB
struct BoundingBox::BoundingBox RTree::randomBB(float from, float to, int dimension){
    vector<float> min;
    vector<float> max;
    for(int i = 0; i < dimension; i++){
        float m = randomFloat(from, to);
        float ma = 0;
        do{
            ma = randomFloat(from, to);
        } while (ma < m);
        min.push_back(m);
        max.push_back(ma);
    }
    struct BoundingBox bb(min, max);
    return bb;
}


//generate a vector of BoundingBoxes
vector<struct BoundingBox::BoundingBox> RTree::randomBBvector(float from, float to, int amount, int dimension){
    vector<struct BoundingBox> vec;
    for(int i = 0; i < amount; i++){
        vec.push_back(randomBB(from, to, dimension));
    }   
    return vec;
}


//print a BoundingBox
void RTree::printBB(struct BoundingBox bb){
    for(int i = 0; i < bb.min.size(); i++)
        cout << bb.min[i] << " ";
    cout<< endl;
    for(int i = 0; i < bb.min.size(); i++)
        cout << bb.max[i] << " ";
    cout << endl;
}

// test1 : insert the same element in the tree
void RTree::test1(struct Node &root, struct BoundingBox &elem){
    cout << "root " << root.id << " datasize " << root.data.size() <<" childNode size " << root.childNode.size() <<endl;;
    for(int i = 0; i < 4; i++)
        insert(root, elem);
    // root overflows -> split
    cout << endl << endl << endl << "INSERT : root overflows -> split" << endl;
    for(int i = 0; i < 4; i++){
        insert(root, elem);
    }
    // leaf overflows -> reinsert
    cout << endl << endl << endl << "INSERT : Leaf overflows first time -> reinsert" << endl;
    for(int i = 0; i < 4; i++)
        insert(root, elem);
    // leaf 12 overflows -> split again (root has 4 entries)
    for(int i = 0; i < 4; i++)
        insert(root, elem);
    // leaf 12 overflows -> split and propagate overflowTreatment upwards
    cout << endl << endl << endl << "INSERT : propagate overflowTreatment upwards" << endl;
  /*  for(int i = 0; i < 15; i++)
        insert(root, elem);
    // leaf 12 overflows -> split and propagate overflowTreatment upwards two levels
    cout << endl << endl << endl << "INSERT : propagate overflowTreatment upwards" << endl;
    for(int i = 0; i < 4; i++)
        insert(root, elem);
    // leaf 12 overflows -> reinsert the entries
 //    cout << endl << endl << endl << "REINSERT :" << endl;
    for(int i = 0; i < 15; i++)
        insert(root, elem);
    for(int i = 0; i < 14; i++)
        insert(root, elem);
    cout << endl << endl << endl << "INSERT : root overflows -> split to 4 levels" << endl;
    insert(root, elem);*/
    insert(root, elem);
    // reinsert must be possible because an new level was created in the las insert
    cout<< "reinsert possible: " << reins;

    // print final tree and the inserted element
    cout <<endl << endl << endl << "FINAL TREE: \n\nelem :" << root.id << endl;
    printBB(elem);
    cout << " insertID :" << endl;
    printTreeID(root);
    cout << " insert :" << endl;
    printTree(root);
}


// test2 : insert randome element in the tree
void  RTree::test2(struct Node &root, int from, int to, int amount, int dimension){
    vector<struct BoundingBox > bboxes = randomBBvector(from, to, amount, dimension);
   // for(auto &i: bboxes)
     //   cout<< &i << endl;
    for(int i = 0; i < bboxes.size(); i++){
        insert(root, bboxes[i]);
    }
    // print final tree and the inserted element
    cout <<endl << endl << endl << "FINAL TREE: \n " << root.id << endl;
    cout << " insertID :" << endl;
    printTreeID(root);
    cout << " insert :" << endl;
    printTree(root);
    cout << endl << endl << "THE BOXES ARE" << endl;
    for(auto &i: bboxes){
        cout<< &i << endl;
        printBB(i);
    }
  //  for(auto &i: bboxes)
    //    cout<< &i << endl;
    struct Node * leafNode= findLeaf(root, &bboxes[0]);
    cout << endl << endl << "THE LEAF NODE IS: " << leafNode->id << endl << &leafNode << endl;
}

template <typename T, typename Compare> std::vector<std::size_t> RTree::sort_permutation(const std::vector<T>& vec,
    Compare compare) {
    std::vector<std::size_t> p(vec.size());
    std::iota(p.begin(), p.end(), 0);
    std::sort(p.begin(), p.end(),
        [&](std::size_t i, std::size_t j){ return compare(vec[i], vec[j]); });
    return p;
}

template <typename T> void RTree::apply_permutation_in_place(std::vector<T>& vec, const std::vector<std::size_t>& p) {
    std::vector<bool> done(vec.size());
    for (std::size_t i = 0; i < vec.size(); ++i)
    {
        if (done[i])
        {
            continue;
        }
        done[i] = true;
        std::size_t prev_j = i;
        std::size_t j = p[i];
        while (i != j)
        {
            std::swap(vec[prev_j], vec[j]);
            done[j] = true;
            prev_j = j;
            j = p[j];
        }
    }
}

template <typename T> std::vector<T> RTree::apply_permutation(const std::vector<T>& vec, const std::vector<std::size_t>& p) {
    std::vector<T> sorted_vec(vec.size());
    std::transform(p.begin(), p.end(), sorted_vec.begin(),
        [&](std::size_t i){ return vec[i]; });
    return sorted_vec;
}

bool wayToSort(int i, int j) { return i > j; }

void RTree::init(int from, int to, int amount, int dimension){
    vector<float> min1 = {2,2};
    vector<float> max1 = {3,4};
    vector<float> min2 = {1,1};
    vector<float> max2 = {4,4};
    vector<float> min3 = {4,4};
    vector<float> max3 = {6,5};
    vector<float> min4 = {7,6};
    vector<float> max4 = {9,7};
    vector<float> min5 = {2,2};
    vector<float> max5 = {5,4};
    struct BoundingBox bb1(min1, max1);
    struct BoundingBox bb2(min2, max2);
    struct BoundingBox bb3(min3, max3);
    struct BoundingBox bb4(min4, max4);
    struct BoundingBox bb5(min5, max5);

    // make method createRandomData/rectangles
    vector<struct BoundingBox> data = {bb1, bb2, bb3, bb4, bb5};
    vector<struct BoundingBox *> dataAdress = {&bb1, &bb2, &bb3, &bb4, &bb5};


    // test distance
   // cout << distanceBB(bb1, bb2) << endl;

  //  struct Node rtree = createRTree(data);
    struct Node rtree(data, dataAdress);
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
/*
    //test createBB
    vector<struct BoundingBox> bb = createBB(4, 0, 11, 2);
    for(auto &i : bb){
        cout << i.min[0] << " ";
    }
    cout << endl;
*/  
/*
   // test overlap
   cout << "overlap: " << overlap(bb1, bb2) << endl;
   cout << "overlap: " << overlap(bb1, bb3) << endl;
   */
  
    vector<float> minVec  = {4,6};
    vector<float> maxVec = {5,7};
    struct BoundingBox elem(minVec, maxVec);
    struct Node test(data, dataAdress);
    struct Node a(data, dataAdress);


    struct Node root;
    // test 1: always insert the same element
  //  test1(root, elem);

  /*  vector<float> minVec1  = {4,7};
    vector<float> maxVec1 = {5,7};
    struct BoundingBox elem1(minVec1, maxVec1);
    cout<< endl << endl << endl << "INSERT LARGER ELEMENT" << endl;
    insert(root, elem1);
    vector<float> minVec2  = {2,7};
    vector<float> maxVec2= {2,7};
    struct BoundingBox elem2(minVec2, maxVec2);
    insert(root, elem2);
    vector<float> minVec3  = {6,9};
    vector<float> maxVec3 = {7,11};
    struct BoundingBox elem3(minVec3, maxVec3);
    insert(root, elem3);
    printTreeID(root);
    printTree(root);
*/
    //test 2: insert random elements
    test2(root, from, to, amount, dimension);



//    cout << " pointer of root " << &root << endl;
  //  cout << " pointer of bb3 " << &bb3 << endl;
  //  deleteEntry(root, &bb3);

 //   cout << "findLeaf " << findLeaf(n2, &bb1) << " " << endl;


 /*   // test findEntries
    cout << "find Entries: " << endl;
    vector<struct BoundingBox *> findentries = findEntries(n2);
    for(auto &j : findentries)
            cout << j->min[0] << " " << j->min[1] << " " << j->max[0] << " " << j->max[1] <<" " << &j << endl;
*/
/*
    // test min overlap enlargement
    cout << "min overlap enlargement: " << endl;
    for(auto &i : minOverlap(test, elem))
        cout << i->min[0] << " " << i->min[1] << " " << i->max[0] << " " << i->max[1] << endl;
*/

// test choose subtree
/*
    struct Node * subtree = chooseSubtree(test, elem);
    cout << endl << endl;
    for(auto &i : subtree->entries)
        cout << i.min[0] << " " << i.min[1] << " " << i.max[0] << " " << i.max[1] << endl;
*/
/*

    cout << "node: " << endl;
    for(auto &i : test.entries)
        cout << i.min[0] << " " << i.min[1] << " " << i.max[0] << " " << i.max[1] << endl;
    
    vector<struct Node> nodevec = split(test);
    struct Node n = nodevec[0];
    struct Node m = nodevec[1];

    cout << "split first group: " << endl;
    for(auto &i : n.entries)
        cout << i.min[0] << " " << i.min[1] << " " << i.max[0] << " " << i.max[1] << endl;
    cout << "split second group: " << endl;
    for(auto &i : m.entries)
        cout << i.min[0] << " " << i.min[1] << " " << i.max[0] << " " << i.max[1] << endl;
 */

}