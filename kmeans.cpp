#include <iostream>
#include <random>
#include <stdlib.h>
#include <limits.h>
using namespace std;


float randomFloat(float min, float max) {
    return (min + 1) + (((float) rand()) / (float) RAND_MAX) * (max - (min + 1)); 
}

void initClusters();

struct Point {
    float x;
    float y;
};

struct Cluster {
    int number;

    //Liste von Mittelwerten
    struct Point* middles;
};

struct Cluster* createCluster(int number){
     //Allocating space to struct Cluster and Points
    struct Cluster* c = (struct Cluster*) malloc (sizeof(struct Cluster));
    c->middles = (struct Point*) malloc(c->number * sizeof(struct Point));
    c->number = number;
};

// Menge an Punkten
struct Set {
    int amount;
    
    // Liste von Punkten
    struct Point* points;
};

struct Set* createSet(int amount)
{
    struct Set* set = (struct Set*) malloc(sizeof(struct Set));
    //Allocating space to struct set

    set->amount = amount;
    set->points = (struct Point*) malloc(set->amount * sizeof(struct Point));
    //creating Edge type structures inside Graph structure, the number of edge type
    //structures is equal to number of edges 
}

void argmin(Point* p){
    int min = INT_MAX;
    int mitte;


}

void initClusters(struct Cluster* clus){
    for(int i = 0; i < clus->number; i++){
        clus->middles[i].x = randomFloat(1,5);
        clus->middles[i].y = randomFloat(1,5);

    }

}

void alg(struct Set* set)
{
    bool change = true;
    while(change){
        for(int i = 0; i < set->amount; i++){
            argmin(&(set->points[i]));
        }
    }
}


main() {
    // Anzahl an Punkten; leere Liste erstellen
    int number = 3;
    struct Set* test  = createSet(5);

    // Punkte erstellen, an Liste anhängen
    test->points[0].x = 1.1;
    test->points[0].y = 1.1;
    test->points[1].x = 1.3;
    test->points[1].y = 1.4;
    test->points[2].x = 2.3;
    test->points[2].y = 2.6;
    test->points[3].x = 3.0;
    test->points[3].y = 2.1;
    test->points[4].x = 4.6;
    test->points[4].y = 3.3;
    test->points[5].x = 3.9;
    test->points[5].y = 2.4;

    struct Cluster* clus = createCluster(number);
    initClusters(clus);

    for(int i = 0; i < number ; i++){
        cout<< clus->middles[i].x << "\t" << clus->middles[i].y << "\n";
    }

  


}