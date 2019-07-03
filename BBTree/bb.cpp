#include <iostream>
#include <vector>
#include <random>

#include "bb.hpp"

using namespace std;

// constructors
BB::BB(int bmax) : bmax(bmax), isSuper(false){};

BB::BB(int bmax, vector<vector<float> > elements, vector<vector<float> * > tids) : bmax(bmax), isSuper(false), elements(elements), tids(tids) {};


//methods

// input: regular BB -> determine how many entires it contains
int BB::amountRegularEntries(){
    return tids.size();
}

// input: super BB -> determine how many entires it contains
int BB::amountSuperEntries(){
    int counter = 0;
 //   cout<< endl << "amount super entries " << endl;
    for(auto &i : buckets){
        for(auto &j : i.elements){
            counter++;
        }
    }
  //  cout<< counter;
    return counter;
}

// intput: super BB -> determine which child to choose when inserting in a super BB
int BB::chooseSuperChild(vector<float> &element, int k){
    int child = 0;
    cout << endl << "choose super child" << endl;
    for(auto &i : delimiterValues)
        cout<< i << "\t";
        cout<< endl << "elem: " << endl;
    for(auto &i : element)
        cout<< i << "\t";
        cout<< endl << delimiterValues.size() << endl << element[0] << endl << delimiterValues[1] << endl;
    for(int i = 0; i < delimiterValues.size(); i++){
        cout << "i: " << i << " " << element[delimiterDim] << " " << delimiterValues[i] << endl;
        if(element[delimiterDim] > delimiterValues[i]){
          cout << "is larger " << endl;
            child = i + 1;
        } else {
            cout << "not larger" << endl;
        }
    }
    cout << "child " << child ;
    return child;
}


// print a regular
void BB::printRegular(){
    cout<< endl << "REGULAR BB: ";
    for(auto &j : elements){
        for(auto &k : j)
            cout<< k << "\t";
        cout<< endl;
    }
    cout<< "tids: " << endl;
    for(auto &j : tids){
        cout<< j << "\t";
    }
    cout<< endl;
}

// print a super BB
void BB::printSuper(){
    cout<< endl << "SUPER BB: " << endl << "delimiter dim: " << delimiterDim << endl << "delimiter Values: ";
    for(auto &i: delimiterValues)
        cout<< i << "\t";
    for(int i = 0; i < buckets.size(); i++){
        cout<< endl << "bucket " << i << endl << "elements: " << endl;
        for(auto &j : buckets[i].elements){
            for(auto &k : j)
                cout<< k << "\t";
            cout<< endl;
        }
        cout<< "tids: " << endl;
        for(auto &j : buckets[i].tids){
                cout<< j << "\t";
        }
        cout<< endl;
    }
}



