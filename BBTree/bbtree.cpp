#include <iostream>
#include <vector>
#include <random>
#include <bits/stdc++.h>
#include <math.h>
#include <queue> 
#include <future>
#include <thread>


#include <cstdlib>
#include <chrono>

#include "bbtree.hpp"
#include "bb.hpp"

using namespace std;


BBTree::BBTree(int k, int bmax) : k(k), bmax(bmax) {};


//random Float erstellen
float BBTree::randomFloat(float min, float max) {
    return (min + 1) + (((float) rand()) / (float) RAND_MAX) * (max - (min + 1)); 
}


template <typename T, typename Compare> std::vector<std::size_t> BBTree::sort_permutation(const std::vector<T>& vec,
    Compare compare) {
    std::vector<std::size_t> p(vec.size());
    std::iota(p.begin(), p.end(), 0);
    std::sort(p.begin(), p.end(),
        [&](std::size_t i, std::size_t j){ return compare(vec[i], vec[j]); });
    return p;
}

template <typename T> void BBTree::apply_permutation_in_place(std::vector<T>& vec, const std::vector<std::size_t>& p) {
    std::vector<bool> done(vec.size());
    for (std::size_t i = 0; i < vec.size(); ++i){
        if (done[i]){
            continue;
        }
        done[i] = true;
        std::size_t prev_j = i;
        std::size_t j = p[i];
        while (i != j){
            std::swap(vec[prev_j], vec[j]);
            done[j] = true;
            prev_j = j;
            j = p[j];
        }
    }
}

template <typename T> std::vector<T> BBTree::apply_permutation(const std::vector<T>& vec, const std::vector<std::size_t>& p) {
    std::vector<T> sorted_vec(vec.size());
    std::transform(p.begin(), p.end(), sorted_vec.begin(),
        [&](std::size_t i){ return vec[i]; });
    return sorted_vec;
}


// the cardinalities of each dimension in an array sorted int descending order
vector<int> BBTree::cardinalityDimensions(vector<vector<float> > vec){
    int delimiterDim = 0;
    int maxDistValues = INT_MIN;
    vector<int> cardinalities;
    // the dimensions which are going to be ordered at the end according to their cardinalities and returned
    vector<int> dimensions;
    for(int i = 0; i < vec[0].size(); i++)
        dimensions.push_back(i);
    cout<< "dimensions: " << endl;
    for(auto &i : dimensions)
        cout<< i << "\t" << endl;

    // calculate for each dimension the sum of distinct values
    for(int i = 0; i < vec[0].size(); i++){
        vector<float> tmp;
        // for each element of the vector
        for(int j = 0; j < vec.size(); j++){
                tmp.push_back(vec[j][i]);
        }
        sort(tmp.begin(), tmp.end());
        cout<< "tmp: " << endl;
        for(auto &k : tmp)
            cout<< k << "\t" << endl;
        cardinalities.push_back(unique(tmp.begin(), tmp.end()) - tmp.begin());
        cout << "cardinality at " << i << cardinalities[i] << endl; 
    } 

    // sort the dimensions according to their cardinalities and return the vector
    auto p = sort_permutation(cardinalities, [](const int & c1, const int & c2){
            return c1 > c2;
        });
    dimensions = apply_permutation(dimensions, p); 
    return dimensions;
}


// get the dimension with the largest number of distinct values
int BBTree::getDelimiterDim(vector<vector<float> > vec){
    int delimiterDim = 0;
    int maxDistValues = INT_MIN;
    // calculate for each dimension the sum of distinct values
    for(int i = 0; i < vec[0].size(); i++){
        vector<float> tmp;
        // for each element of the vector
        for(int j = 0; j < vec.size(); j++){
                tmp.push_back(vec[j][i]);
        }
        sort(tmp.begin(), tmp.end());
        int distValues = unique(tmp.begin(), tmp.end()) - tmp.begin();
     /*   for(auto &j : tmp)
            cout << j << "\t";
        cout<< "dist Values " << distValues;
        cout<< endl;
    */
   
   // find the dimension with the max number of distinct values
        if(distValues > maxDistValues){
            maxDistValues = distValues;
            delimiterDim = i;
        }
    } 
    cout<< "delimiter Dimension: " << delimiterDim << endl;
    return delimiterDim;
}


void BBTree::testGetDelimiterDim(){
    vector<vector<float> > vec;
    vector<float> a = {1,2,3};
    for(int i = 0; i < 16; i++){
        vec.push_back(a);
    }
    vec[0][1] = 4;
    getDelimiterDim(vec);
}


// morph a regular BB into a super BB
void BBTree::intoSuper(struct BB &bb){
    cout<< "elements: " << endl;
    for(auto &i : bb.elements){
        for(auto &j : i)
            cout<< j << "\t";
        cout<< endl;
    }
    cout<< "tids: " << endl;
    for(auto &i : bb.tids)
        cout<< i << "\t";
    cout<< endl;
    // bb becomes super BB
    bb.isSuper = true;

    // determine delimiter dimension
    bb.delimiterDim = getDelimiterDim(bb.elements);
    // order the vector and the tids the same way alongside the delimiter Dimension
    int axis = bb.delimiterDim;
    auto p = sort_permutation(bb.elements, [ axis ](const vector<float> & v1, const vector<float> & v2){
            return v1[axis] < v2[axis];
        });
    // sort the elements and the tids the same way
    bb.elements = apply_permutation(bb.elements, p);
    bb.tids = apply_permutation(bb.tids, p); 

// split the vector of elements of the BB into k (approximately) equal parts
    vector<vector<vector<float> > > splitElem = splitVector(bb.elements, k);
    // determine the delimiter values -> the highest value of a bucket in the current dimension
    for(auto &i : splitElem){
        // i = current bucket; retrieve last element of the bucket, append the value of the current dimension which we use for
           // splitting to the solution
     bb.delimiterValues.push_back(i[i.size()-1][bb.delimiterDim]);
    }
    // we don´t need the last element
        bb.delimiterValues.pop_back();
     cout<< "solution delimiterValues vector: " << endl;
        for(auto &k : bb.delimiterValues)
            cout<< k << "\t";
        cout<< endl;

    for(int i = 0; i < k; i++){
        struct BB tmp(4);
        bb.buckets.push_back(tmp);
    }
    cout << "buckets size: " << bb.buckets.size() << endl;

    // simply insert the elements/tids according to the delimiter values -> don´t split them extra
    for(int i = 0; i < bb.elements.size(); i++){
        // the child of the super BB is the new BB we want to insert into -> insert
            int child = bb.chooseSuperChild(bb.elements[i], k);
            // append to the TID and to the actual data vector
            bb.buckets[child].tids.push_back(bb.tids[i]);
            bb.buckets[child].elements.push_back(bb.elements[i]); 
    }
    cout << "elements size: " << bb.elements.size() << endl;
    
    // delete array of regular BB
    for(auto &i : bb.elements)
        bb.elements.pop_back();
    for(auto &i : bb.tids)
        bb.tids.pop_back();
}


// how many data elements are int the tree
int BBTree::amountData(vector<struct BB> &bbvec){
    int n = 0; 
    for(auto &i : bbvec){
        if(!i.isSuper)
            n += i.amountRegularEntries();
        else
            n += i.amountSuperEntries();
    }
    return n;
}


void BBTree::printQueue(queue<vector<vector<float> > > &q){
    queue<vector<vector<float> > > tmp_q = q; //copy the original queue to the temporary queue
int counter = 0;
    while (!tmp_q.empty()) {
        vector<vector<float> > q_element = tmp_q.front();
        cout<< endl << "element " << counter << endl;
        for(auto &i : q_element){
            for(auto &j : i)
                cout << j << "\t";
            cout << endl;
        }
        tmp_q.pop();
        counter++;
    }
}


///// no need for buckets right with current implementation of amount of buckets
// use breadth first search traversal to get the delimiter values
vector<float> BBTree::BreadthFirstTravseral(vector<vector<float> > &sample, int buckets, vector<int> dimensions){
    cout << endl << endl << "breadth first traversal of the ist:" << endl;
    queue<vector<vector<float> >  > q;
    // solution vector with the delimiter Values/inner search tree
    vector<float> solIst;

 /*   if (sample.empty()) {
        return;
    }
 */   //while -> for all delimiter values/ everything inside in if counter < delimiters.size()
    q.push(sample);
    printQueue(q);
    int counter = 0;
    // must traverse all nodes -> determine how many there will be
    int end = 0;
    for(int i = 0; i < dimensions.size(); i++){
        end += pow(3,i);
    }
    cout << "end: " << end << endl;
//replace end for 1
    while(!q.empty() && counter < end){
        int level = heightTree(counter+1)-1;
        cout<< "counter : " << counter+1 << " level: " << level << endl;
   // for (q.push(root); !q.empty(); q.pop()) {
        vector<vector<float> > currentElem = q.front();
        cout << "front of the queue:  " << endl;
        for(auto &i : currentElem){
            for(auto &j : i)
                cout << j << "\t";
            cout << endl;
        }
        // traverse the current node
        // append all the elements of the sample vector of the current dimension
        vector<float> tmp;
        for(int j = 0; j < currentElem.size(); j++){
                tmp.push_back(currentElem[j][dimensions[level]]);
        }
        cout<< "elements of the current dimension: " << dimensions[level] << " size of currentElement: " << tmp.size() << endl;
        for(auto &j : tmp)
                cout << j << "\t";
            cout << endl;
        // sort the dimension according to its cardinalities
        auto p = sort_permutation(tmp, [](const float & c1, const float & c2){
                return c1 < c2;
            });
        // sort the current Element accordingly
        currentElem = apply_permutation(currentElem, p);
        cout<< "after sort (elements of the current dimension): " << dimensions[level] << " size of currentElement: " << tmp.size() << endl;
        for(auto &j : currentElem)
                cout << j[dimensions[level]] << "\t";
            cout << endl;
        // divide the sorted data into buckets -> split the vector of sample elements into k (approximately) equal parts
        vector<vector<vector<float> > > splitElem = splitVector(currentElem, k);
        cout<< endl << "the new buckets are: ";
        for(int i = 0; i <splitElem.size(); i++){
            cout<< endl << "bucket " << i << endl;
            for(auto &j : splitElem[i]){
                for(auto &k : j)
                    cout<< k << "\t";
                cout<< endl;
            }
        }

        // determine the delimiter values -> the highest value of a bucket in the current dimension
        for(auto &i : splitElem){
            // i = current bucket; retrieve last element of the bucket, append the value of the current dimension which we use for
            // splitting to the solution
            solIst.push_back(i[i.size()-1][dimensions[level]]);
        }
        // we don´t need the last element
        solIst.pop_back();
        cout<< "solution IST vector: " << endl;
        for(auto &k : solIst)
            cout<< k << "\t";
        cout<< endl;

        // append the elements generated through the split at the end of the queue
        for(auto &i : splitElem){
            q.push(i);
        }
        q.pop();
        cout<< endl << endl << endl<< "THE NEW QUEUE IS: " << endl;
        printQueue(q);
        counter++;
    }
    return solIst;
}

//get all the tids
vector<vector<float> > BBTree::getBBtids(vector<struct BB> &bbvec){
    vector<vector<float> * > allData;
    for(auto &i: bbvec){
        if(!i.isSuper){
            for(auto &j : i.tids){
                allData.push_back(j);
            }
        } else {
            for(auto &k : i.buckets){
                for(auto &j : k.tids){
                    allData.push_back(j);
                }
            }
        }
    } 
}

// get all the elements from the Leaf nodes of a BB tree stored in a BB vector
vector<vector<float> > BBTree::getBBelements(vector<struct BB> &bbvec){
    vector<vector<float> > allData;
    for(auto &i: bbvec){
        if(!i.isSuper){
            for(auto &j : i.elements){
                allData.push_back(j);
            }
        } else {
            for(auto &k : i.buckets){
                for(auto &j : k.elements){
                    allData.push_back(j);
                }
            }
        }
    } 
    return allData;
}


// reorganize index if super BB is overflows
void BBTree::reorganizeIndex(vector<float> &ist, vector<int> &delimiters, vector<struct BB> &bbvec){
    // 1. determine how many regular BB are needed to hold the current data, while leaving capacity for new inserts -> n/(10% * bmax)??? -> why not simply increase by one level
   // int buckets = amou = ntData(bbvec)/(0.25 * bmax);
    // determine the amount of levels necessary to build the tree
    // -> height of tree with root not counted
   // int levels = heightTree(n);
   // int levels = ceil(log(buckets)/log(k))-1;

    // for reorganization -> increase level in tree by one
    int levels = delimiters.size() + 1;
    int buckets  = pow(3,levels);
    cout << "amount buckets: " << buckets << " amount levels: " << levels << endl;

    // 2. randomly sample Rsample * n dataobjects as representatives of the whole data set
    //retrieve all the data and tids from the BB vector
    vector<vector<float> > dataObjects = getBBelements(bbvec);
    cout<< "data objects: " << endl;
    for(auto &i : dataObjects){
        for(auto &j : i)
             cout << j << "\t";
        cout << endl;
    }

    vector<vector<float> > sample; // you don't have to use this since the chosen ones will be in the back of the vector
    // determine the correct sample size !!!!!!!!!!!!!!!!!!!!!
    for(int i = 0; i < 0.5*dataObjects.size(); ++i) {
        int index = rand()%((dataObjects.size() - i - 1) - 0 + 1) + 0;
        sample.push_back(dataObjects[index]);
        swap(dataObjects[index], dataObjects[dataObjects.size() - i - 1]);
    }
    cout<< "sample: " << endl;
    for(auto &i : sample){
        for(auto &j : i)
             cout << j << "\t";
        cout << endl;
    }

    // scan data set -> estimate cardinality of each dimension -> vector with cardinalities in descending order
    delimiters = cardinalityDimensions(sample);
    cout << "delimiters: " << endl;
    for(auto &j : delimiters)
        cout << j << "\t";
    cout << endl;

    // the sorted cardinalities are assigned to the h IST levels in descending order after clearing the delimiters
    cout << "levels: " << levels << endl;
    vector<int> tmp(delimiters.begin() + 0, delimiters.begin()+ levels);
    delimiters = tmp;
    cout << "delimiters" << endl;
    for(auto &j : delimiters)
        cout << j << "\t";
    cout << endl;

    // 3. recursively determine the delimiter values for the inner nodes -> using sample data, determine compute an equi-depth histogram with
    // k buckets, reflecting the distribution of the dimension values of the current level
    // - get k-1 delimiter valuses such that data are divided into k partitions of app. equal size
    ist.clear();
    ist.shrink_to_fit();
    ist = BreadthFirstTravseral(sample, buckets, delimiters);
    cout << "inner search tree: " << endl;
    for(auto &j : ist)
        cout << j << "\t";
    cout << endl;

    // all objects are inserted into their respective BB -> extract the data from the existing BB
    vector<vector<float> * > data;
    for(auto &i : bbvec){
        if(!i.isSuper){
            for(auto &j : i.tids)
                data.push_back(j);
        } else{
            for(auto &j : i.buckets){
                for(auto &k : j.tids)
                    data.push_back(k);
            }

        }
    }
    cout<< " data " << (*data[0])[0] << endl << "extracted data from the existing BB" << endl;
     for(auto &i : data){
        for(auto &j : *i)
             cout << j << "\t";
        cout << endl;
    }
   // vector<float> &ist, vector<int> &delimiters, vector<struct BB> &bbvec
   cout << "ist: " << endl;
    for(auto &j : ist)
        cout << j << "\t";
    cout << endl;
    cout << "delimiters: " << endl;
    for(auto &j : delimiters)
             cout << j << "\t";


   // clear the BB and IST vector, insert anew
    for(auto &i : bbvec)
        bbvec.pop_back();
    for(int i = 0; i < buckets; i++){
        struct BB tmp(bmax);
        bbvec.push_back(tmp);
    }
    cout << endl << "size of bbvec: " << bbvec.size() << " buckets " << buckets << endl;
    for(auto &i : data)
        insert(ist, delimiters, bbvec, *i);
}


// traverse ist for insertion -> determine correct Leaf Node
// return -> index of location in bbvec
int BBTree::traverseIst(vector<float> &ist, vector<int> &delimiters, vector<BB> &bbvec, vector<float> &element){
    int position = 0;
    int level = 0;
    int index = 0;
    // for each level: determine which child node to choose next until the last level of the array is reached
    while(level < delimiters.size()){
//        cout << "LEVEL " << level << endl; 
        // traverse the node
        // for inner node in current level: determine childNode to take -> add the previous points *3 * 2 to jump in next level
        int childNode = 1;
//        cout << 0 << "\t" << element[delimiters[level]] << " " << ist[position] << endl;
        for(int i = 1; i < k; i++){
//            cout << i << "\tchildnode: " << childNode << "\t" << element[delimiters[level]] << " " << ist[position + i - 1] << endl;
            // check wether the coordinate of the level is larger than the delimiter value -> childNode we use in the next step
            if(element[delimiters[level]] > ist[position + i - 1]){
//                cout << "is larger " <<childNode<< endl;
                childNode = i + 1;
            }
        }
        // after node in current level is explored -> jump to childNode in the next level
//        cout <<  "position  " << position <<  " k-1 " << k-1 << " childnode " << childNode << " k " << k << endl;
        position = (position/(k-1) * k + childNode) * (k-1);
        level++;
    }
    for(int i = 0; i < level; i++){
        cout << index << endl;
        index += pow(3,i);
    }
    return position/(k-1) - index;
}


// insert an element into the tree
void BBTree::insert(vector<float> &ist, vector<int> &delimiters, vector<struct BB> &bbvec, vector<float> &element){
    // The BB we choose to insert the element
   struct BB * chosenBB;
    
    // if we have no BB to insert -> create initial empty regular BB -> chosenBB
    if(bbvec.empty()){
        // evtl mit new ????
        cout<< "bbvec is empty" << endl;
        struct BB bb(bmax);
        bbvec.push_back(bb);
        printBBvec(bbvec);
    }
    // traverse the ist to determine the relevant BB
    chosenBB = &bbvec[traverseIst(ist, delimiters, bbvec, element)];
    cout << "the chosen BB is : " << chosenBB << " isSuper: " << chosenBB->isSuper << endl;
    for(int i = 0; i < bbvec.size(); i++){
        if(&bbvec[i] == chosenBB)
            cout << "chosen BB on place " << i << endl;;
    }

    // if BB is regular
   if(!chosenBB->isSuper){
       cout<< "the chosen bb is regular ";
        // if regular and free -> insert
        if(chosenBB->elements.size() < chosenBB->bmax){
            cout << "size " << chosenBB->tids.size() << " " << &element;
            cout<< "and free " << endl;
            // append to the TID and to the actual data vector
            chosenBB->tids.push_back(&element);
            chosenBB->elements.push_back(element);
            chosenBB->printRegular();
        // if regular and not free -> morph into supper BB
        } else
            intoSuper(*chosenBB);
   }
   // if BB is super
   if(chosenBB->isSuper){
       cout<< "the chosen bb is super ";
        // if super and contains less than k * bmax objects -> determine appropriate child node
        if(chosenBB->amountSuperEntries() < k * chosenBB->bmax){
            cout<< " and free -> insert" << endl;
            // the child of the super BB is the new BB we want to insert into -> insert
            int child = chosenBB->chooseSuperChild(element, k);
            chosenBB = &chosenBB->buckets[child];
            // append to the TID and to the actual data vector
            chosenBB->tids.push_back(&element);
            chosenBB->elements.push_back(element);
            cout << "elements:" << endl;
            for(auto &i : chosenBB->elements)
                cout << i[0] << "\t" << endl;
                cout << endl ;
        // otherwise: reorganize index
        } else {
            cout<<" and not free -> call reorganize Index" << endl;
            reorganizeIndex(ist, delimiters, bbvec);
            cout<< "index is reorganised -> insert element anew " << endl;
            insert(ist, delimiters, bbvec, element);
        }
   }
}

// fix: currently 3-dimensional
vector<vector<float> > BBTree::createData(int from, int to, int dimensions, int amount){
    vector<vector<float> > data;
    for(int i = 0; i < amount; i++)
            data.push_back({randomFloat(from, to), randomFloat(from, to), randomFloat(from, to)});
    return data;
}


void BBTree::printData(vector<vector<float> > &data){
    cout << "DATA :\n\n";
    for(auto &i : data){
        for(auto &j : i)
            cout<< j << "\t";
        cout<< "\n";
    }
    cout << endl;

}

void BBTree::printIst(vector<float> &ist, vector<int> &delimiters){
    cout << "INNER SEARCH TREE: " << endl;
    int pos = 0;
    for(auto &i : ist)
    cout << i << "\t";
    cout << endl;
 /*   for(int i = 0; i < delimiters.size(); i++){
        int counter = 0;
        while(counter < pow(k,i)){
            for(int j = 0; j < delimiters.size()-1; j++){
                cout << ist[pos++] <<"\t";
            }
            counter++;
        }
        cout<< endl;
    }*/
}

// does not print super yet
void BBTree::printTree(vector<float> &ist, vector<int> &delimiters, vector<BB> &bbvec){
    printIst(ist, delimiters);
    cout << endl << "LEAVE NODES:" << endl;
    int counter = 0;
    for(auto &i : bbvec){
        cout << "Bucket " << counter << "\t";
        for(const auto &j : i.elements){
            for(const auto &k : j)
                cout << k << "\t";
            cout << endl;
        }
        cout << endl;
    }

}


template<typename T> vector<vector<T>> BBTree::splitVector(const vector<T>& vec, size_t n) {
    vector<vector<T>> outVec;

    size_t length = vec.size() / n;
    size_t remain = vec.size() % n;
    size_t begin = 0;
    size_t end = 0;

    for(size_t i = 0; i < min(n, vec.size()); ++i) {
        end += (remain > 0) ? (length + !!(remain--)) : length;
        outVec.push_back(vector<T>(vec.begin() + begin, vec.begin() + end));
        begin = end;
    }
    return outVec;
}


struct BB * BBTree::findData(vector<float> &ist, vector<int> &delimiters, vector<struct BB> &bbvec, vector<float> &element){

}


// gives back the position of the BB in the bbvec that possibly contain elements in the range search query
vector<int> BBTree::completeRangeQueryIndexes(vector<float> &ist, vector<int> &delimiters, vector<struct BB> &bbvec, vector<float> &element1, vector<float> &element2, int level, int position){
    cout << "ist: " << endl;
    for(auto &i : ist)
        cout << i << "\t";
        cout << endl << "bbvec.size : " << bbvec.size() << endl;
    // abbruchBedingung Rekursion: Leaf level reached:
    vector<int> sol;
    if(delimiters.size() == 0 || level == delimiters.size()-1){
        cout<< "level == delimiters.size() -> Rekursionsabbruch;" << endl;
            // if there are no delimiters by which we can split -> take the single BB at index 0
        if(delimiters.size() == 0){
            sol.push_back(0);
            cout<< "pushback BB at index 0" << endl;
        } else {
            vector<int> childNodes;
            float x = 0;
            float y = 0;
            if(element1[delimiters[level]] <= element2[delimiters[level]]){
                x = element1[delimiters[level]];
                y = element2[delimiters[level]];
            } else{
                y = element1[delimiters[level]];
                x = element2[delimiters[level]];
            }
            cout << "x: " << x << " y: " << y << endl;
            int lower = INT_MIN;
        int upper = INT_MIN;
        if(x <= ist[position] || y <= ist[position]){
            cout<< "comparison 0: " << endl;
            if(x <= ist[position]){
                lower = 1;
                cout << "lower: " << lower << endl;
            }
            if(y <= ist[position]){
                upper = 1;
                cout << "upper: " << upper << endl;
            }
        }
        cout << "first  iteration" << endl;
           // if(x <= ist[position]){
             //   cout << "delimiters level: " << delimiters[level] << " ist pos: " << position << " " << ist[position] << endl;
               // sol.push_back(position/(k-1) - index);
            //}
            for(int i = 1; i < k; i++){
                // check wether the delimiter Value of the level is larger than the value of e1 and smaller than e2 -> proceed with childNode
                if(x > ist[position + i - 1] || lower != INT_MIN && y > ist[position + i - 1]){
                    if(x > ist[position + i - 1]){
                        lower = i + 1;
                        cout << "lower: " << lower << " i " << i << endl;
                    } 
                    if(lower != INT_MIN && y > ist[position + i - 1]){
                        upper = i + 1;
                        cout << "upper: " << upper << " i " << i << endl;
                    }
                    cout << "is larger " << "level: " << level << endl;
                    // ?????
                  //  sol.push_back(position/(k-1) - index + i);
                } else { 
                    cout << "is not larger"<< endl;
                }
            }
            for(auto &i: childNodes)
                cout << i << "\t";
             for(int i = lower; i <= upper; i++){
                childNodes.push_back(i);
            }
            for(auto &i: childNodes)
                cout << i << "\t";
            cout << "lower: " << lower << "upper: " << upper << endl;
            cout << "iterarion terminated: lower: " << lower << "upper: " << upper << endl;
            position = (position/(k-1) * k + childNodes[0]) * (k-1);
            cout << "position " << position << endl;
            int index = 0;
            for(int i = 0; i <= level; i++){
                cout << index << endl;
                index += pow(3,i);
            }
            cout << "level: " << level <<  "index: " << index << "childNodes.size(): " << childNodes.size() << " childNodes: " << endl;
            for(auto &i: childNodes)
                cout << i << "\t";
            for(int i = 0; i < childNodes.size()*(k-1); i++){
                if(i%2 == 0)
                    sol.push_back((position + i)/(k-1) - index);
            }    
        }
        cout << endl << "solution indexes: " << endl;
        for(auto &i: sol)
            cout << i << "\t";
        return sol;
    }
    // for each level: determine which child node to choose next until the last level of the array is reached
        cout << "rekursion step \nLEVEL " << level << endl; 
        cout<< "delimiters.size " << delimiters.size() << endl << "delimiters: " << endl;
        for(auto &i : delimiters)
            cout << i;
            cout << endl;
        cout<< "elements:" << endl;
        for(int i = 0; i < element1.size(); i++){
            cout << "e1: " << element1[i] << " e2: " << element2[i] << endl;
        }
        cout<< "IST:" << endl;
        for(auto &i : ist){
            cout << i << "\t";
        }
        cout << endl;
    // traverse the node
    // for inner node in current level: if the delimiter is between the two values of e1 and e2 of the current dimension -> proceed with childNode
   
   // x is the smaller number -> for the comparisons with the delimiter values
        float x = 0;
        float y = 0;
        if(element1[delimiters[level]] <= element2[delimiters[level]]){
            x = element1[delimiters[level]];
            y = element2[delimiters[level]];
        } else{
            y = element1[delimiters[level]];
            x = element2[delimiters[level]];
        }
        cout << "x: " << x << " y: " << y << "position: " << position << endl;
   
    vector<int> childNodes;
    int lower = INT_MIN;
    int upper = INT_MIN;
    if(x <= ist[position] || y <= ist[position]){
        cout<< "comparison 0: " << endl;
        if(x <= ist[position]){
            lower = 1;
            cout << "lower: " << lower << endl;
        }
        if(y <= ist[position]){
            upper = 1;
            cout << "upper: " << upper << endl;
        }
        cout << "proceed with childnode " << "delimiters level: " << delimiters[level] << " element 1 level: " << element1[delimiters[level]]<< "ist pos: " << ist[position] << endl;
    }       

    for(int i = 1; i < k; i++){
     //       cout << i << "\tchildnode: " << childNode << "\t" << element[delimiters[level]] << " " << ist[position + i - 1] << endl;
        // check wether the delimiter Value of the level is larger than the value of e1 and smaller than e2 -> proceed with childNode
        cout << "position value:  " << ist[position + i - 1] << " pos: " << position + i - 1 << endl;
        if(x > ist[position + i - 1] || lower != INT_MIN && y > ist[position + i - 1]){
            if(x > ist[position + i - 1]){
                lower = i + 1;
                cout << "lower: " << lower << endl;
            } 
            if(lower != INT_MIN && y > ist[position + i - 1]){
                upper = i + 1;
                cout << "upper: " << upper << endl;
            }
            cout << "proceed with childnode " << "delimiters level: " << delimiters[level] << "ist pos: " << ist[position] << endl;
            cout << "is larger " << endl;
        } else { 
            cout << "is not larger"<< endl;
        }
    }
    cout << "iterarion terminated: lower: " << lower << "upper: " << upper << endl;
    for(int i = lower; i <= upper; i++){
        childNodes.push_back(i);
    }
    cout<< "childNodes: " << endl;  
    for(auto &i : childNodes)
        cout << i << "\t";
    cout << endl;

    
    // after node in current level is explored -> jump to childNode in the next level
        cout <<  "position  " << position <<  " k-1 " << position/(k-1) * k << " childnode " << childNodes[0] << " k " << k << endl;
    for(int i = 0; i < childNodes.size(); i++){
        int childPosition = (position/(k-1) * k + childNodes[i]) * (k-1);
        cout << "childPosition  " << childPosition << " childNodes size: " << childNodes.size() << endl;
        vector<int> tmp = completeRangeQueryIndexes(ist, delimiters, bbvec, element1, element2, level + 1, childPosition);
        sol.insert(sol.end(), tmp.begin(), tmp.end());
    }
    cout << endl << endl << "SOLUTION: " << endl;
    for(auto &i : sol)
        cout << i << "\t";
        cout << endl;
    return sol;
}

 
//gets a potential BB that may contain relevant elemants for the complete/partial range query
vector<vector<float> * > checkBB(int id, struct BB solBB, vector<float> * element1, vector<float> * element2){
    cout << endl << endl << "THREAD ID: " << id << endl << endl << "issuper: " << solBB.isSuper;
    cout << "element 1: " << endl;
    for(auto &i : *element1)
        cout << i << "\t";
    cout << endl;
    cout << "element 2: " << endl;
    for(auto &i : *element2)
        cout << i << "\t";
    cout << endl;
    cout << "tids: " << endl;
        for(auto &k : solBB.tids){
            for(auto &i : *k)
                cout << i << "\t";
            cout << endl;
    }
    vector<vector<float> * > solAdresses;
    // if the passed BB is a super BB -> call a Thread for each normal BB it contains
    if (solBB.isSuper){
        cout << "isSuper" << endl;
        int counter = 0;
        for(auto &i : solBB.buckets){
            future<vector<vector<float> * > > th  = async(&checkBB, id, i, element1, element2);
            vector<vector<float> * > tmp = th.get();
            solAdresses.insert(solAdresses.end(), tmp.begin(), tmp.end());
            //counter++;
            cout << "solution adresses: " << endl;
            for(auto &k : solAdresses){
                for(auto &i : *k)
                    cout << i << "\t";
                cout << endl;
            }
        }
    // else -> check for each element in the regular BB if it is part of the solution
    } else {
        cout << "isRegular" << endl;
        // check for each element  of the current BB if it fits into the range of the elements
        for(int j = 0; j < solBB.elements.size(); j++){
            cout<< "element of solBB nr.: " << j << endl;
            bool isPart = true;
            for(int i = 0; i < element1->size(); i++){
                
                // check for each dimension if it fits into the values of the dimension of the elements
                float x = 0;
                float y = 0;
                if(element1->at(i) <= element2->at(i)){
                    x = element1->at(i);
                    y = element2->at(i);
                } else{
                    y = element1->at(i);
                    x = element2->at(i);
                }
                cout << "x: " << x << " y " << y << endl;
                // if the current value is not part of the range -> don´t append it to the solution
                cout << "solBB.elements[j][i] " << solBB.elements[j][i]  << endl;
                if(solBB.elements[j][i] < x || solBB.elements[j][i] > y)
                    isPart = false;
                    cout << "isPart: " << isPart << endl;
            }
            if(isPart){
                solAdresses.push_back(solBB.tids[j]);
            }
            cout << "solution adresses: " << endl;
            for(auto &k : solAdresses){
                for(auto &i : *k)
                    cout << i << "\t";
                cout << endl;
            }
        }
    }
    return solAdresses;

}


// finds a range query in a tree
vector<vector<float> * > BBTree::completeRangeQuery(vector<float> &ist, vector<int> &delimiters, vector<struct BB> &bbvec, vector<float> &element1, vector<float> &element2){
    vector<int> bbVecIndexes = completeRangeQueryIndexes(ist, delimiters, bbvec, element1, element2, 0,0);
    // create a thread for each BB to check if the elements inside are wanted:
    // not wanted: if e.g. 3. dimension is not in between the 3. dimension of the two elements
    vector<vector<float> * > solAdresses;
    int counter = 0;
    cout << "bbvec size" << bbvec.size() << endl;
    for(int i = 0; i < bbVecIndexes.size(); i++){
        cout << endl << endl << "bbvec nr. : " << i << endl;
        future<vector<vector<float> * > > th  = async(&checkBB, counter, bbvec[bbVecIndexes[i]], &element1, &element2);
        // return the adresses of the points we are searching for
        vector<vector<float> * > tmp = th.get();
        solAdresses.insert(solAdresses.end(), tmp.begin(), tmp.end());
        counter++;
    }
    cout << "solution adresses complete range query: " << endl;
        for(auto &k : solAdresses){
            for(auto &i : *k)
                cout << i << "\t";
            cout << endl;
        }
    return solAdresses;
}



vector<int> BBTree::partialRangeQuery(vector<float> &ist, vector<int> &delimiters, vector<struct BB> &bbvec, vector<float> &element1, vector<float> &element2, int level, int position){

}


// height of the tree with edge count
int BBTree::heightTree(int ist){
    cout << " ist " << ist << " k " << k << endl;
    int counter = 0;
    int i = 0;
    while(counter < ist){
        counter += pow(k,i); 
        cout << "counter " << counter << endl;
        i++;
    }
    return i;
}


void BBTree::printBBvec(vector<struct BB > bbvec){
    cout << endl << "THE BBVEC IS:" << endl << endl;
    int counter = 0;
    for(auto &i : bbvec){
        cout << "BB NUMBER " << counter << ":";
        if(i.isSuper)
            i.printSuper();
        else 
            i.printRegular();
        counter ++;
    }
}


void BBTree::init(int from, int to, int dimensions, int amount){
    // initialize vector with the data points
    vector<vector<float> > data = createData(from, to, dimensions, amount);
    printData(data);

    // initialize vector of IST and of delimiters
    vector<float> ist;
    vector<int> delimiters;

    // initialise BB vector
    vector<BB> bbvec;

    //  TEST insert the first data element into the tree
    insert(ist, delimiters, bbvec, data[0]);
    cout<< endl << endl << "insert 1 completed " << endl << endl;
    insert(ist, delimiters, bbvec, data[1]);
    cout<< endl << endl << "insert 2 completedd " << endl << endl;
    insert(ist, delimiters, bbvec, data[2]);
    cout<< endl << endl << "insert 3 completedd " << endl << endl;
    insert(ist, delimiters, bbvec, data[3]);
    cout<< endl << endl << "insert 4 completedd " << endl << endl;
    insert(ist, delimiters, bbvec, data[3]);
    cout<< endl << endl << "insert 4 completedd " << endl << endl;
    insert(ist, delimiters, bbvec, data[3]);
    insert(ist, delimiters, bbvec, data[4]);
    insert(ist, delimiters, bbvec, data[5]);
    insert(ist, delimiters, bbvec, data[4]);
    insert(ist, delimiters, bbvec, data[4]);
    insert(ist, delimiters, bbvec, data[4]);
    insert(ist, delimiters, bbvec, data[4]);
    printIst(ist, delimiters);
    printBBvec(bbvec);
   // cout<< endl << endl << "FIRST SUPER BB OVERFLOWS" << endl << endl;
   // insert(ist, delimiters, bbvec, data[4]);
    insert(ist, delimiters, bbvec, data[2]);
    insert(ist, delimiters, bbvec, data[4]);
    insert(ist, delimiters, bbvec, data[3]);
    insert(ist, delimiters, bbvec, data[6]);
    insert(ist, delimiters, bbvec, data[7]);
    cout<< endl << endl << "SECOND SUPER BB OVERFLOWS" << endl << endl;
    insert(ist, delimiters, bbvec, data[4]);
    insert(ist, delimiters, bbvec, data[8]);
    insert(ist, delimiters, bbvec, data[9]);
    insert(ist, delimiters, bbvec, data[1]);
    insert(ist, delimiters, bbvec, data[9]);
    insert(ist, delimiters, bbvec, data[6]);

    printIst(ist, delimiters);
    printBBvec(bbvec);
    
    // TEST complete range query
    // level: at which level we are in the inner search tree
    // position: at which position we are in the bbtree array
    cout<< endl << endl << "TEST complete Range Query: " << endl;
    vector<float> is = {2,6,1,4,3,5,3,7};
    vector<int> delimiter = {0,1};
    vector<float> e1 = {1,4,1};
    vector<float> e2 = {12,10,7};
    completeRangeQuery(ist, delimiter, bbvec, e1, e2);
    vector<vector<float> > bbelems = getBBelements(bbvec);
    cout << "all inserted elements in the bbvec:" << endl;
    for(auto &j : bbelems){
        for(auto &i : j)
            cout << i << "\t";
        cout << endl;
    }
    cout << "size: " << bbvec.size() << endl;


  // TEST checkBB
//  cout<< endl << endl << "TEST checkBB: " << endl;
 // vector<vector<float> * > testCheckBB = checkBB(1, bbvec[0], &e1, &e2);
 /* for(auto &i : testCheckBB){
      for(auto &j : *i)
        cout << j << "\t";
    cout << endl;
  }*/


    // TEST split vector
/*    vector<int> vec= {1,2,3,4,5};
    vector<vector<int> > ve = splitVector(vec, 3);
    for(auto &i : ve){
        for(auto &j : i)
            cout << j << "\t";
            cout << endl;
    }
*/






    // TEST into Super
 /*   vector<vector<float> > data1 = createData(from, to, dimensions, 20);
    vector<vector<float> * > datatids;
    for(auto &i : data1)
        datatids.push_back(&i);
    struct BB bb(bmax, data1, datatids);
    bb.printRegular();
    //intoSuper(bb);
    //bb.printSuper();
*/

   // vector<vector<float> > &sample, int buckets, vector<int> delimiters
    // TEST determine the delimiter values after reorganizing the index -> with Breadth-first search traversal
    // n : number of buckets needed to hold the data
  /*   int n = bb.amountRegularEntries()/(0.1 * bmax);
    int levels = ceil(log(n)/log(k))-1;
    vector<int> del;
    for(int i = 0; i < levels; i++)
        del.push_back(i);
    cout<< "number of buckets: " << n << endl << "levels " << levels <<  " delimiters: " << endl ;
    for(auto &i : del)
        cout << i;
    vector<int> delimiterValues = BreadthFirstTravseral(data1, n, del);
*/

    // TEST height of tree
   // cout << endl << heightTree(5) << endl;


    // TEST Get Dilimiter Dimension
    //testGetDelimiterDim();

    // TEST traverse IST
  //  ist = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12};
   // ist = {0,1,2,3,4,5,6,7,8,9,10,11,12,13};
  //  vector<int> del = {0,1,2};
 /*   vector<float> element = {1,2,2};
    for(auto &i : element)
        cout << i << "\t";
    cout << endl;
    cout << traverseIst(ist, del, bbvec, element) << endl;
 */ 
    // TEST print IST
    //printIst(ist, del);
    

}
