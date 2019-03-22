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
    c->number = number;
    c->middles = (struct Point*) malloc(c->number * sizeof(struct Point));
};

// Menge an Punkten
struct Set {
    int amount;
    
    // Liste von Punkten
    struct Point* points;
    struct Point** clusters;
};

struct Set* createSet(int amount)
{
    // Allocating space to struct set
    struct Set* set = (struct Set*) malloc(sizeof(struct Set));

    set->amount = amount;
    set->points = (struct Point*) malloc(set->amount * sizeof(struct Point));
    set->clusters = (struct Point**) malloc(set->amount * sizeof(struct Point*));
}

// Distanz zwischen zwei Punkten
float dist(struct Point* p1, struct Point* p2){
    return sqrtf(pow(p1->y - p2->y, 2) + pow(p1->x - p2->x, 2));
 }

 // Test dist
void testDist(struct Point* p1, struct Point* p2){
    float abst = dist(p1, p2);
    std::cout<< "\nAbstand zwischen zwei Punkten:\t" << abst << "\n";
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

    // Test argmin
void testArgmin(struct Point* p, struct Cluster* clus){
    struct Point* punkt = argmin(p, clus);
    cout<< "\n Test Argmin: \n";
    for(int i = 0; i < clus->number; i++){
        float d = dist(p, &clus->middles[i]);
        cout<< "Abstand:\t" << d << "\n";
    }
}

// Printed Cluster
void printClusters(struct Cluster* clus){
    cout<< "\nClusters:\nx\ty\n";
    for(int i = 0; i < clus->number; i++){
        cout<< clus->middles[i].x << "\t" << clus->middles[i].y << "\n";
    }
}

// Printed ein set von Punkten
void printSet(struct Set* set){
    cout<< "\nPoints:\t\tClusters:\nx\ty\tx\ty\n";
    for(int i = 0; i < set->amount ; i++){
    cout<< set->points[i].x << "\t" << set->points[i].y << "\t" << set->clusters[i]->x << "\t" << set->clusters[i] << "\n";
    }
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

// Array erstellen Punkte mit ihren nächsten Clustern
void createArr(struct Set* set, struct Cluster* clus){
    for(int i = 0; i < set->amount; i++){
      set->clusters[i] = argmin(&set->points[i], clus);
    }
}

// Summiert für jedes Cluster die zugehörigen Punkte auf + Summe der Kluster Skalieren
void sumclus(struct Set* set, struct Cluster* clus){
    for(int i = 0; i < clus->number; i++){
        float sumx = 0;
        float sumy = 0;
        int anzahlP = 0;
        for(int j = 0; j < set->amount; j++){
            cout<< "\nWerte: "<< j << ": " << set->clusters[j] << "\t" <<&clus->middles[i];
            if(set->clusters[j] == &clus->middles[i]){
                anzahlP += 1;
                sumx += set->points[j].x;
                sumy += set->points[j].y;
                // Print zu welchem Kluster ein Punkt gehört
             //  cout<< "\n"<< j << "\tSumme: " << sumx << "\t" << sumy << "\t" << argmin(&set->points[j],clus)->x << "\t" <<  argmin(&set->points[j],clus)->y << "\t" << clus->middles[i].x;
            } 
       /*    else {
                cout<< "\n"<< j << "\tfalse ";
           } */
        }
        // neue Cluster: skaliert durch # an zugehörigen Punkten (wenn keine Punkte dazugehören -> 0/1 = 0)
        if (anzahlP == 0){
            clus->middles[i].x = 0;
            clus->middles[i].y = 0;
        } else {
        clus->middles[i].x = sumx/anzahlP;
        clus->middles[i].y = sumy/anzahlP;
        }
        cout<< "\nAnzahl der zugehörigen Punkte:\t" << anzahlP << " \tneue Cluster Werte:\t" << clus->middles[i].x << "\t" << clus->middles[i].y; 
    }
}

// eigentlicher Algorithmus
void alg(struct Set* set, struct Cluster* clus)
{
    bool changec = true;

    // solange sich die Cluster verändern
    while(changec){
        changec = false;

        // für jeden Punkt das Cluster mit dem geringsten Abstand berechnen
        for(int i = 0; i < set->amount; i++){
            struct Point* minclus = argmin(&set->points[i], clus);
            if (minclus != set->clusters[i]){
               set->clusters[i] = minclus;
                changec = true;
            }
        }

        //Cluster neu berechnen (Summe über alle Punkte, die zum gleichen Cluster gehören)
        // + Summe der Kluster Skalieren
        sumclus(set, clus);
    }
    // Ergebnisse der Cluster printen
    cout<< "Die Ergebniscluster sind:\n";
    printClusters(clus);
}

// Punkte erstellen
void createPoints(struct Set* set){
    set->points[0].x = 1;
    set->points[0].y = 2;
    set->points[1].x = 2;
    set->points[1].y = 3;
    set->points[2].x = 3;
    set->points[2].y = 4;
    set->points[3].x = 6;
    set->points[3].y = 6;
    set->points[4].x = 9;
    set->points[4].y = 5;
    set->points[5].x = 8;
    set->points[5].y = 8;
}

main() {
    // # Cluster -> leere Liste von Clustern erstellen
    int numberClus = 3;
    struct Cluster* clus = createCluster(numberClus);

    // Cluster initiieren; Printen; den Punkten zuordnen -> evtl in alg?????
    initClusters(clus);
    printClusters(clus);

    // # Punkte -> leere Liste von Punkten erstellen
    int amountPoints = 5;
    struct Set* set  = createSet(5);

    // Punkte initiieren mit zugehörigen Clustern; Printen
    createPoints(set);
    createArr(set, clus);
    printSet(set);

    // k-means Algorithmus
    alg(set, clus);
}
