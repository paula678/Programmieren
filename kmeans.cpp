#include <iostream>
#include <random>
#include <stdlib.h>
#include <limits.h>
#include <cstdlib>
#include <tgmath.h>
#include <array>
using namespace std;


//random Float erstellen (funktioniert nicht wirklich???)
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
    float min = INT_MAX;
    for(int i = 0; i < clus->number; i++){
        float abst = dist(p, &clus->middles[i]);
        if(abst < min){
            min = abst;
            zentrum = &clus->middles[i];
        }
    }
    return zentrum;
}

// Cluster mit Zufallszahlen initiieren
void initClusters(struct Cluster* clus){
    clus->middles[0].x = 1.1;
    clus->middles[0].y = 1.1;

    for(int i = 1; i < clus->number; i++){
     //   clus->middles[i].x = randomFloat(1,5);
     //   clus->middles[i].y = randomFloat(1,5);
    clus->middles[i].x = clus->middles[i-1].x + 0.5;
    clus->middles[i].y = clus->middles[i-1].x + 0.5;
    }
}

void sumclus(struct Point* ergarr[][2], int arrlen, struct Cluster* clus){
    for(int i = 0; i < clus->number; i++){
        float sumx = 0;
        float sumy = 0;
        for(int j = 0; j < arrlen; j++){
            if(ergarr[j][1] == &clus->middles[i]){
                sumx += ergarr[j][0]->x;
                sumy += ergarr[j][0]->y;
                // Print zu welchem Kluster ein Punkt gehört
              //  cout<< "\n"<< j << "\tSumme: " << sumx << "\t" << sumy << "\t" << argmin(ergarr[j][0],clus)->x << "\t" <<  argmin(ergarr[j][0],clus)->y << "\t" << clus->middles[i].x;
            } 
        /*   else {
                cout<< "\n"<< j << "\tfalse ";
           } */
        }
        clus->middles[i].x = sumx;
        clus->middles[i].y = sumy;
    }
}

// eigentlicher Algorithmus
void alg(struct Set* set, struct Cluster* clus)
{
    // Ergebnisarray: zu jedem Pounkt das zugehörige Cluster; initialisierung -> Cluster: null
    struct Point* ergarr[set->amount][2];
    for(int i = 0; i < set->amount; i++){
        ergarr[0][i] = &set->points[i];
    }
    bool changec = true;

    // solange sich die Cluster verändern
    while(changec){
        changec = false;

        // für jeden Punkt das Cluster mit dem geringsten Abstand berechnen
        for(int i = 0; i < set->amount; i++){
            Point* minclus = argmin(&set->points[i], clus);
            if (minclus != ergarr[1][i]){
                ergarr[1][i] = minclus;
                changec = true;
            }
        }

        //Cluster neu berechnen (Summe über alle Punkte, die zum gleichen Cluster gehören)
    }
}


main() {
    // Anzahl an Punkten; leere Liste erstellen
    int number = 3;
    struct Set* test  = createSet(5);

    // Punkte erstellen, an Liste anhängen
    test->points[0].x = 1;
    test->points[0].y = 2;
    test->points[1].x = 2;
    test->points[1].y = 3;
    test->points[2].x = 3;
    test->points[2].y = 4;
    test->points[3].x = 6;
    test->points[3].y = 6;
    test->points[4].x = 9;
    test->points[4].y = 5;
    test->points[5].x = 8;
    test->points[5].y = 8;

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
  /*
    // Test dist
    float abst = dist(&test->points[0], &test->points[4]);
    cout<< "\nAbstand zwischen zwei Punkten:\n" << abst << "\n";
*/
    // Test argmin
 /*   struct Point* punkt = argmin(&test->points[0], clus);
    cout<< "\nargmin:\nx:\ty:\n" << punkt->x << "\t" << punkt->y << "\n";
    float d1 = dist(&test->points[0], &clus->middles[0]);
    float d2 = dist(&test->points[0], &clus->middles[1]);
    float d3 = dist(&test->points[0], &clus->middles[2]);
    cout<< "\nAbstand c0:" << d1 << "\tAbstand c1:" << d2 << "\tAbstand c2:" << d3;
*/
    // Test sumclus 
    struct Point* ergarr[test->amount][2];
    for(int i = 0; i < test->amount; i++){
        ergarr[i][0] = &test->points[i];
        ergarr[i][1] = argmin(ergarr[i][0], clus);
    }
    int arrlen = sizeof(ergarr)/sizeof(ergarr[0]);
    sumclus(ergarr, arrlen, clus);

    // Test Clusterwerte
    cout<< "\nClusters:\nx\ty\n";
    for(int i = 0; i < number ; i++){
        cout<< clus->middles[i].x << "\t" << clus->middles[i].y << "\n";
    }



    
}