#include <vector>
#include <iostream>
#include <algorithm>

#include "art.hpp"
#include "node.hpp"

using namespace std;

main() {
    cout << "Hello World" << endl;
 /*
    int value = 4;
    int * y = &value;
    int ** v = &y;
    struct ART a;
    struct Node ** x;
     *x = 0;
    a.x(v);*/

    // TEST create Node
   // vector<int> keys = {1,5,3,6,7,2,10, 8};
   // vector<int> keys = {1,5,3,6};
 /*     vector<int> keys;
    for(int i = 0; i < 48; i ++)
        keys.push_back(i);
    struct Node c1;
    struct Node c2;
    struct Node c3;
    struct Node c4;
    struct Node c5;
    struct Node c6;
    struct Node c7;
    struct Node c8;*/
    /*cout << &c1 << "\t";
    cout << &c2 << "\t";
    cout << &c3 << "\t";
    cout << &c4 << "\t";
    cout << &c5 << "\t";
    cout << &c6 << "\t";
    cout << &c7 << "\t";
    cout << &c8 << "\t" << endl;*/
   // vector<struct Node *> childNodes = {&c1,&c2,&c3,&c4, &c5,&c6,&c7,&c8};
   // vector<struct Node *> childNodes = {&c1,&c2,&c3,&c4};
 /*   vector<struct Node *> childNodes;
   for( int i = 0; i < 48; i++)
       childNodes.push_back(&c1);
   cout << "print Nodes:" << endl;
    struct Node n(keys, childNodes);
    n.printNode();

    // TEST getKeys
    vector<int> getk = n.getKeys();
    cout << "TEST getKeys" << endl;
    for(auto &i : getk)
        cout << i << "\t";
        cout << endl;

    // TEST isFull
    cout << "isFull: "  << n.isFull() << endl;

    // TEST overflow
    n.overflow();
    cout << endl << "TEST overflow" << endl;
    n.printNode();

    // TEST addChild
    cout << endl << "TEST addChild " << 4 << " " << &c6 << endl;
    n.addChild(4, &c6);
    n.printNode();
*/
    // TEST insert
    cout << endl << "TEST insert "  << endl;
    struct ART a;
    // adress to an adress for a node
    struct Node *t = 0;
    struct Node ** tree = &t;
    // the adress of the node is 0 -> empty tree, not initialised yet -> do this in init function in art
 /*   int64_t test = 1;
    vector<uint8_t> testKey = a.makeKeyvector(test);
    for(auto &i: testKey)
        cout << (int)i << "\t" ;
    a.insert(tree, testKey, &test , 0);
    a.printTree(*tree, 0);
    
    int64_t test2 = 2;
    vector<uint8_t> testKey2 = a.makeKeyvector(test2);
    for(auto &i: testKey2)
        cout << (int)i << "\t" ;
    a.insert(tree, testKey2, &test2 , 0);
    (*tree)->printNode();
    int64_t test3 = 3;
    vector<uint8_t> testKey3 = a.makeKeyvector(test3);
    for(auto &i: testKey3)
        cout << (int)i << "\t" ;
    a.insert(tree, testKey3, &test3 , 0);
    a.printTree(*tree, 0);*/

    vector<vector<uint8_t> > values;
    vector<int64_t> valueelements;
    for(int i = 0; i < 2; i++){
        values.push_back(a.makeKeyvector((int64_t) i));
        valueelements.push_back((int64_t) i);
    }
    values.push_back(a.makeKeyvector((int64_t) 300));
        valueelements.push_back((int64_t) 300);
    
    for(int i = 0; i < 3; i++) {
       // values.push_back(a.makeKeyvector(valueelements[i]));
        a.insert(tree, values[i], &valueelements[i], 0);
     //   (*tree)->printNode();
      //  valueelements.push_back(valueelements[i]+1);
    }

    cout << endl << endl << "PRINT TREE: " << endl << endl;
    a.printTree(*tree, 0);
    

  /*  cout << endl << endl << "PRINT END TREE CHILDS: " << endl;
    for(auto &i : (*tree)->childNodes){
        i->printNode();
      //  cout << "value " << i << " " << a.getValue(*i);
    }*/
 /*   cout << endl << endl << "PRINT END TREE CHILD VALUES: " << endl;
    for(auto &i : (*tree)->childNodes[1]->childNodes){
       // i->printNode();
        cout << " value " << i << " " << a.getValue(*i);
    }*/
    


/* //TEST convert an int into an array of length 4 -> keys are 32 bit long???
    vector<int> arrayOfByte(4);
     for (int i = 0; i < 4; i++)
         arrayOfByte[4-1-i] = (3000 >> (8*i)) & 0xff;
     for(auto &i : arrayOfByte)
        cout << i << "\t" << endl;
*/


}
