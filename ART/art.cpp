#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <string.h>


#include "art.hpp"
#include "node.hpp"

using namespace std;

ART::ART(){}


struct Node ** ART::findChild(struct Node &node, uint8_t byte) {
    // NODE 4 -> simple loop
    cout << endl << endl << "FIND CHILD  byte: "  << (int) byte << endl;
    if(node.nodetype == 0){
        cout << "node 4 " << endl;
        for(auto &i : node.keys){
            cout << (int) i << "\t";
        }
        cout << endl;
        for(auto &i : node.keys){
            cout << "keys: " << (int) i << " byte  " << (int) byte << endl;
            if(((int) i) == ((int) (byte))){
                cout << endl << endl << "FIND CHILDNODE: " << node.childNodes[i] << endl;
                return &node.childNodes[i];
            } 
        }
        return NULL;

    // NODE 16 -> SSE comparison
    } else if(node.nodetype == 1){
       /* char c = (char) byte;
        __m128i keysse = _mm_set1_epi8(c);
        // use the correct part of the key of the node -> add the keys of the node (complete prefix) for comparison 
        // evtl last key of prefix
        __m128i nodekeysse = _mm_cvtsi32_si128 (node.prefix[0]);
        __m128i cmp = _mm_cmpeq_epi8(keysse, nodekeysse);
        int mask = (1<<node.getKeys().size())-1;
        int bitfield = _mm_movemask_epi8(cmp)&mask;
        // was kommt bei bitfield raus
        if(bitfield) {
            return node.childNodes[__builtin_ctz(bitfield)];
        } else {
            return NULL;
        }*/
        for(int i = 0; i < node.getKeys().size(); i++){
            if(node.keys[i] == byte){
                cout << endl << endl << "FIND CHILDNODE: " << node.childNodes[i];
                return &node.childNodes[i];
            } 
        }
        return NULL;

    // NODE 48 -> two array lookups
    } else if(node.nodetype == 2){
        // make a vector of integers -> one byte long? as indexes for childNodes vector
        if(!node.keys48[byte] == 0){
            cout << endl << endl << "FIND CHILDNODE: " << node.childNodes[node.keys48[byte]-1];
            return &node.childNodes[node.keys48[byte]-1];
        } else {
            return NULL;
        }

    // NODE 256 -> one array lookup
    } else if(node.nodetype == 3){
        cout << endl << endl << "FIND CHILDNODE: " << node.keys[byte];
        return &node.keys256[byte];
    }

}

/*template <typename T> void ART::x(const T * node) {
        cout << "nodepointer " << node << endl;

}*/

void ART::x(int  ** node) {
        cout << "nodepointer " << node << "node pointer " << *node << endl;
}


// of which data type should the leaf be -> pointer or int? -> make generic?
// leaf node: use pointer tagging/ store value with additional bit
// key splitted in array, depth starts at null (count upwards), determine lealf type
/*
void ART::insert(struct Node ** nodepointer, vector<uint8_t> key, int64_t * leafvalue, int depth){
    cout << "nodepointer " << nodepointer <<  " node " << *nodepointer  <<endl;
    cout << "leaf " << leafvalue << endl;
    // cast the leaf adress into a node adress -> set the lat bit 1
    struct Node * leaf = saveLeaf(leafvalue);
    cout << "casted leaf " << leaf << endl;
    cout << "casted leaf value " << getValue(*leaf) << endl;
    // stop recursion if depth 8 is reached -> insert last element
    if(depth >= key.size()/8-1){
        cout << "add child int node: " << endl;
        if((*nodepointer)->isFull()){
            (*nodepointer)->overflow();
        }
        (*nodepointer)->addChild(key[depth], leaf);
        return;
    }

    // recursion step

    // handle empty tree -> check if no object
    if(*nodepointer == 0){
        cout<< "handle empty tree: " << endl; 
        // insert the leaf and recurse
        *nodepointer = replace(**nodepointer, leaf);
        cout << "recurse: " << endl;
        insert(nodepointer, key, leafvalue, 0);
        return;
    }


    // if leaf -> expand node -> nodepointer durch richtigen Knoten mit Leaf als Kind ersetzen
    if(getLastBit(*nodepointer) == 1){
        struct Node * newNode = new Node;
        newNode->addChild((uint8_t) key[depth], leaf);
        *nodepointer = newNode;
        insert(nodepointer, key, leafvalue, depth+1);
    }



    // node -> search for child
    else {
        struct Node * child = findChild(**nodepointer, key[depth]);
        cout << "child: " << child << endl;
        // child is found
        if(child != 0){
            cout << "recurse: " << endl;
            insert(&child, key, leafvalue, depth+1);

        // no child is found - add to inner node
        } else {
             cout << "add child int node: " << endl;
            if((*nodepointer)->isFull()){
                (*nodepointer)->overflow();
            }
            (*nodepointer)->addChild(key[depth], leaf);

        }

        // if no child is found
    }

    // if the node is a leaf -> expand node
    if(getLastBit(*nodepointer) == 1){
        cout << "node is leaf -> expand node: " << endl;
        // create a new node for expanison
        struct Node * newNode = new Node;
        cout << "new node: " << newNode << " key : ";
        vector<uint8_t> key2 = loadKey(**nodepointer);
        cout << " " << key2.size() << endl;
        for(auto &i : key2)
            cout << (int) i << "\t";
            cout << endl;
        newNode->addChild((uint8_t) key[depth], leaf);
        cout << "print childNode 0 of newNode: " << endl;
        newNode->childNodes[0]->printNode();
        cout << "print childNode 1 of newNode: " << endl;
        newNode->childNodes[1]->printNode();
        *nodepointer = replace(**nodepointer, newNode);
        *nodepointer = insert(nodepointer, key, leafvalue, depth+1);
        return;
    }
    }

}*/


void ART::insert(struct Node ** nodepointer, vector<uint8_t> key, int64_t * leafvalue, int depth){
      cout << "nodepointer " << nodepointer <<  " node " << *nodepointer  <<endl;
    cout << "leaf " << leafvalue << endl;
    // cast the leaf adress into a node adress -> set the lat bit 1
    struct Node * leaf = saveLeaf(leafvalue);
    cout << "casted leaf " << leaf << endl;
    cout << "casted leaf value " << getValue(*leaf) << endl;

    // handle empty tree -> check if no object
    if(*nodepointer == 0){
        cout<< "handle empty tree: " << endl; 
        // which value do I save?
        *nodepointer = replace(**nodepointer, leaf);
        return;
    }


    //expand node -> if node is leaf: with bits -> implement at the end, evtl not needed
    // if, because of lazy expansion, an existing leaf is encountered, it is replaced by 
    // a new inner node storing the existing and the new leaf
    if(getLastBit(*nodepointer) == 1){
        cout << "node is leaf -> expand node: " << endl;
        // create a new node for expanison
        struct Node * newNode = new Node;
        cout << "new node: " << newNode << " key : ";
        vector<uint8_t> key2 = loadKey(**nodepointer);
        cout << " " << key2.size() << endl;
        for(auto &i : key2)
            cout << (int) i << "\t";
            cout << endl;
        for(auto &i : key)
            cout << (int) i << "\t";
            cout << endl;
        int i;
        for(i = depth; key[i] == key2[i]; i++){
            newNode->prefix.push_back(key[i]);
        }
        /*for(auto &i : newNode->prefix)
            cout << (int) i << "\t";
            cout << endl;*/
        newNode->prefixLen = i-depth;
        depth = depth + newNode->prefixLen;
        cout << "prefixlen: " << newNode->prefixLen << " depth: " << depth << endl;
        // man kann bei mbit kein child hinzufügen
        // cast sollte eigentlich nicht benötigt werden??? -> ist bereits array aus uint8_t
        newNode->addChild((uint8_t) key[depth], leaf);
        newNode->addChild((uint8_t) key2[depth], *nodepointer);
        cout << "print childNode 0 of newNode: " << endl;
        newNode->childNodes[0]->printNode();
        cout << "print childNode 1 of newNode: " << endl;
        newNode->childNodes[1]->printNode();
        cout << "print new node: "  << endl;
        newNode->printNode();
        *nodepointer = replace(**nodepointer, newNode);
        return;
    }

    // prefix mismatch
    // calculate the number of equal bytes
    int p = checkPrefix(*nodepointer, key, depth);
    cout << "check Prefix: " << p  << " prefixLen: " << (*nodepointer)->prefixLen << endl;
    if(p != (*nodepointer)->prefixLen){
        cout << "new Node" << endl;
        for(auto &i : key)
            cout << (int) i << "\t";
            cout << endl;
        struct Node * newNode = new Node;
        newNode->addChild(key[depth+p], leaf);
        newNode->addChild((*nodepointer)->prefix[p], *nodepointer);
    /*    cout << "print childNode 0 of newNode: " << endl;
        newNode->childNodes[0]->printNode();
        cout << "print childNode 1 of newNode: " << endl;
        newNode->childNodes[1]->printNode();
        cout << "print new node: "  << endl;
        newNode->printNode();*/
        newNode->prefixLen = p;
        // copy a block of memory
        memcpy(&newNode->prefix, &(*nodepointer)->prefix, p);
        // change when changing keys to chars
       // for(int i = 0; i < p; i++){
         //  newNode->prefix.push_back((*nodepointer)->prefix[i]);
        //}
        (*nodepointer)->prefixLen = (*nodepointer)->prefixLen-(p+1);
        // move a block of memory
        memmove(&(*nodepointer)->prefix, &(*nodepointer)->prefix+p+1, (*nodepointer)->prefixLen);
      //  for(int i = p+1; i < node->prefixLen; i++){
        //   newNode->prefix.push_back(node->prefix[i]);
        //}
     //   int len = (*nodepointer)->prefixLen;
      //  for(int i = p+1; i < len; i++){
        //   (*nodepointer)->prefix.pop_back();
        //}
        // change when changing keys to chars
        (*nodepointer) = replace(**nodepointer, newNode);
        return;

    }
    
    depth = depth + (*nodepointer)->prefixLen;
    struct Node ** next = findChild(**nodepointer, key[depth]);
    // recurse
    cout << "next :" << next << endl;
    if(next != 0) {
        cout << "recurse: " << endl;
        insert(next, key, leafvalue, depth+1);
    }

    // add to inner node 
    else {
        cout << "add child int node: " << endl;
        if((*nodepointer)->isFull()){
            (*nodepointer)->overflow();
        }
        (*nodepointer)->addChild(key[depth], leaf);
    }
}

void ART::printTree(struct Node* tree, int depth){
    cout << endl << endl << "DEPTH: " << depth << " " << tree << endl;
    tree->printNode();
    if(getLastBit(tree) == 0){
        for(auto &i : tree->getChildNodes()){
            printTree(i, depth+1);
        }
    }

}



struct Node * ART::search(struct Node * node, int key, int depth){
    // empty tree
    if(node = 0){
        // NULL oder 0 returnen? -> evtl Speicherzugriffsfehler
        return NULL;
    }
    // Node is Leaf
    if(getLastBit(node) == 0){}
}

// get byte x of an integer
int ART::getByte(int64_t integ, int x){
    return (integ >> (8*x)) & 0xff;
}

vector<uint8_t> ART::makeKeyvector(int64_t paramInt) {
     vector<uint8_t> arrayOfByte;
     for (int i = 8-1; i >= 0; i--)
        arrayOfByte.push_back(getByte(paramInt, i));
     return arrayOfByte;
}

// replace a node by another one
struct Node * ART::replace(struct Node &node, struct Node * node2){
    return node2;
}


// retrieves key of node from data base -> evtl noch ändern?
// was ist gemeint mit key from data base?
vector<uint8_t> ART::loadKey(struct Node &node){
    if(getLastBit(&node) == 0)
        return node.prefix;
    else if(getLastBit(&node) == 1){
        // shift if we store int values
        return makeKeyvector(getValue(node));
    }
}

// compares compressed path of a node with the key and returns the number equal bytes
int ART::checkPrefix(struct Node * node, vector<uint8_t> key, int depth) {
    // if key has size 4 and we are at the 2. position, we have to compare if the last 2 bytes are equal
    cout << endl << "check prefix" << endl;
    for(auto &i : node->prefix)
        cout << (int) i << "\t" << endl;
    for(auto &i : node->prefix)
        cout << (int) i << "\t" << endl;
    int position = key.size() - depth;
    int compare = min(node->prefixLen, position);
    int index;
    for(index = 0; index < compare; index++) {
        // check if the bytes on the same level are equal
        if(node->prefix[index] != key[depth+index]){
            return index;
        }
    }
}


// delete -> alsready in node class
// get the value of a leaf back that was saved into an adress
int ART::getValue(struct Node &leaf){
    int64_t x =  ((int64_t) (&leaf))>>1;
    int64_t * tmp = (int64_t*) x;
     return (int) *tmp;
}

 // braucht man eigentlich nicht mehr, wenn man Adressen als Blätter speichert
// ist das letzte Bit sowieso 0
// gets an int value and saves it into an adress
struct Node * ART::saveLeaf(void * leaf){
    int64_t x = ((int64_t) leaf)<<1;
    struct Node * tmp = (struct Node *) (x);
    tmp = setLastBit1(tmp);
    return tmp;
}

int64_t ART::getLastBit(struct Node * ptr) {
    return ((int64_t) ptr) & 1;
}

struct Node * ART::setLastBit1(struct Node * ptr) {
    int64_t x = (int64_t) ptr;
    return (struct Node *) (x+1);
}

struct Node * ART::setLastBit0(struct Node * ptr) {
    int64_t x = (int64_t) ptr;
    return (struct Node *) (x-1);
}

void ART::insert_sorted(std::vector<int> & vec, int item ) {
    vec.insert(std::upper_bound( vec.begin(), vec.end(), item ),item );
}




