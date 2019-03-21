#include <iostream>
#include <random>
#include <stdlib.h>
#include <limits.h>
#include <cstdlib>
#include <tgmath.h>
#include <array>
#include <vector>
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
    struct Point* clusters;
};

struct Set* createSet(int amount)
{
    // Allocating space to struct set
    struct Set* set = (struct Set*) malloc(sizeof(struct Set));

    set->amount = amount;
    set->points = (struct Point*) malloc(set->amount * sizeof(struct Point));
    set->clusters = (struct Point*) malloc(set->amount * sizeof(struct Point));
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
    cout<< "Points:\nx\ty\n";
    for(int i = 0; i < set->amount ; i++){
    cout<< set->points[i].x << "\t" << set->points[i].y << "\n";
    }
}

// Printed ein Array
void printArr(struct Set* set){
    cout<< "Points:\nx\ty\n";
    for(int i = 0; i < set->amount; i++){
        cout<< set->points[i].x << "\t" <<  set->points[i].y << "\t" << set->clusters[i].x << "\t" << set->clusters[i].y << "\n";
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
        set->clusters[i] = *argmin(&set->points[i], clus);
    }
}

// Summiert für jedes Cluster die zugehörigen Punkte auf + Summe der Kluster Skalieren
void sumclus(vector<vector<struct Point*>> ergarr, struct Cluster* clus){
    for(int i = 0; i < clus->number; i++){
        float sumx = 0;
        float sumy = 0;
        int anzahlP = 0;
        for(int j = 0; j < ergarr.size(); j++){
            if(ergarr[j][1] == &clus->middles[i]){
                anzahlP += 1;
                sumx += ergarr[j][0]->x;
                sumy += ergarr[j][0]->y;
                // Print zu welchem Kluster ein Punkt gehört
              //  cout<< "\n"<< j << "\tSumme: " << sumx << "\t" << sumy << "\t" << argmin(ergarr[j][0],clus)->x << "\t" <<  argmin(ergarr[j][0],clus)->y << "\t" << clus->middles[i].x;
            } 
        /*   else {
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
/*

// eigentlicher Algorithmus
void alg(struct Set* set, struct Cluster* clus)
{
    // Ergebnisarray: zu jedem Punkt das zugehörige Cluster; initialisierung -> Cluster: null
    vector<vector<struct Point*>> ergarr = createArr(set, clus);
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
        // + Summe der Kluster Skalieren
        sumclus(ergarr, clus);
    }
}
*/

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
    // # Punkte -> leere Liste von Punkten erstellen
    int amountPoints = 5;
    struct Set* set  = createSet(5);

    // Punkte initiieren; Printen
    createPoints(set);
    printSet(set);

    // # Cluster -> leere Liste von Clustern erstellen
    int numberClus = 3;
    struct Cluster* clus = createCluster(numberClus);

    // Cluster initiieren; Printen
    initClusters(clus);
    printClusters(clus);

    // k-means Algorithmus aufrufen
    //alg(set, clus);

    // Ergebnisse der Cluster printen
    printClusters(clus);
    createArr(set, clus);
    printArr(set);
}















/*

// Array erstellen Punkte mit ihren nächsten Clustern
vector<vector<struct Point* >> createArr(struct Set* set, struct Cluster* clus){
    vector<vector<struct Point*>> ergarr(set->amount, std::vector<struct Point*>(2));
    vector<vector<struct Point*>>::const_iterator i;
    vector<struct Point*>::const_iterator j;
    for(i = ergarr.begin(); i != ergarr.end(); ++i){
        (*i).at(0) = &set->points[i];
        ergarr.at(i).at(1) = argmin(ergarr[i][0], clus);
    }
    return ergarr;
}

// Printed ein Array
void printArr(vector<vector<struct Point*>> ergarr){
    vector<vector<struct Point*>>::const_iterator i;
    vector<struct Point*>::const_iterator j;
    cout<< "Points:\nx\ty\n";
    for(i = ergarr.begin(); i != ergarr.end(); ++i){
        for(j = i->begin(); j != i->end(); ++j){
            cout<< (*j)->x << "\t" << (*j)->y;
        }
    }
}

*/