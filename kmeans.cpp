#include <iostream>
#include <random>
#include <stdlib.h>
#include <limits.h>
#include <cstdlib>
#include <tgmath.h>
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
    // Allocating space to struct set
    struct Set* set = (struct Set*) malloc(sizeof(struct Set));

    set->amount = amount;
    set->points = (struct Point*) malloc(set->amount * sizeof(struct Point));
    //creating Edge type structures inside Graph structure, the number of edge type
    //structures is equal to number of edges 
}

 // Distanz zwischen zwei Punkten
 float dist(struct Point * p1, struct Point * p2){
    return sqrtf(pow(p1->y - p2->y, 2) + pow(p1->x - p2->x, 2));
 }

// berechnet für jeden Punkt den Abstand zu jedem Zentrum -> return: Zentrum mit kleinstem Abstand
struct Point* argmin(struct Point* p, struct Cluster* clus){
    struct Point* zentrum;
    int min = INT_MAX;
    for(int i = 0; i < clus->number; i++){
        float abst = dist(p, &clus->middles[i]);
        if(abst < min){
            min = abst;
            zentrum = &clus -> middles[i];
        }
    }
    return zentrum;
}

// Cluster mit Zufallszahlen initiieren
void initClusters(struct Cluster* clus){
    for(int i = 0; i < clus->number; i++){
        clus->middles[i].x = randomFloat(1,5);
        clus->middles[i].y = randomFloat(1,5);
    }
}

// eigentlicher Algorithmus
void alg(struct Set* set, struct Cluster* clus)
{
    bool change = true;
    while(change){
        for(int i = 0; i < set->amount; i++){
            argmin(&(set->points[i]), clus);
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

    // Test Punktewerte
    cout<< "Points:\nx\ty\n";
    for(int i = 0; i < 5 ; i++){
        cout<< test->points[i].x << "\t" << test->points[i].y << "\n";
    }

    // Liste von Clustern erstellen und initiieren
    struct Cluster* clus = createCluster(number);
    initClusters(clus);

    // Test Clusterwerte
    cout<< "\nClusters:\nx\ty\n";
    for(int i = 0; i < number ; i++){
        cout<< clus->middles[i].x << "\t" << clus->middles[i].y << "\n";
    }
    
    // Test dist
    float abst = dist(&test->points[0], &test->points[4]);
    cout<< "\nAbstand zwischen zwei Punkten:\n" << abst << "\n";

    // Test argmin
    struct Point* punkt = argmin(&test->points[0], clus);
    cout<< "\nargmin:\nx:\ty:\n" << punkt->x << "\t" << punkt->y << "\n";
    float d1 = dist(&test->points[0], &clus->middles[0]);
    float d2 = dist(&test->points[0], &clus->middles[1]);
    float d3 = dist(&test->points[0], &clus->middles[2]);
    cout<< "\nAbstand c0:" << d1 << "\tAbstand c1:" << d2 << "\tAbstand c2:" << d3;
    
}