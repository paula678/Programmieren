#include <iostream>
#include <random>
#include <stdlib.h>
#include <limits.h>
#include <cstdlib>
#include <tgmath.h>
#include <vector>

#include "kmeans.hpp"

using namespace std;

int numClus;
int amount;

vector<struct Point*> clusters;

KMeans::KMeans(int c, int p) {
    numClus = c;
    amount = p;
}

//random Float erstellen
float KMeans::randomFloat(float min, float max) {
    return (min + 1) + (((float) rand()) / (float) RAND_MAX) * (max - (min + 1)); 
}

struct Point {
    float x;
    float y;
};

// ordnet Punkte ihren nächsten Clustern zu
void KMeans::assignClus(vector<vector<struct Point*> > &ergset, vector<struct Point> &clus){
    for(auto &i: ergset){
      i[1] = argmin(i[0], clus);
    }
}

// Distanz zwischen zwei Punkten
float KMeans::dist(struct Point* p1, struct Point* p2){
    return sqrtf(pow(p1->y - p2->y, 2) + pow(p1->x - p2->x, 2));
 }

 // Test dist
void KMeans::testDist(struct Point* p1, struct Point* p2){
    float abst = dist(p1, p2);
    std::cout<< "\nAbstand zwischen zwei Punkten:\t" << abst << "\n";
}

// berechnet für jeden Punkt den Abstand zu jedem Zentrum -> return: Zentrum mit kleinstem Abstand
struct Point* KMeans::argmin(struct Point* p, vector<struct Point> &clus){
    struct Point* zentrum;
    float min = INT_MAX;
    for(auto &i: clus){
        float abst = dist(p, &i);
        if(abst < min){
            min = abst;
            zentrum = &i;
        }
    }
    return zentrum;
}

    // Test argmin
void KMeans::testArgmin(struct Point* p, vector<struct Point> &clus){
    struct Point* punkt = argmin(p, clus);
    cout<< "\n Test Argmin: \n";
    for(int i = 0; i <numClus; i++){
        float d = dist(p, &clus[i]);
        cout<< "Abstand:\t" << d << "\n";
    }
}


// Printed Cluster
void KMeans::printClusters(vector<struct Point> &clus){
    cout<< "\nClusters:\nx\ty\n";
    for(const auto &i: clus){
        cout<< i.x << "\t" << &i << "\n";
    }
}

// Printed ein set von Punkten
void KMeans::printSet(vector<vector<struct Point*> > &ergset){
    cout<< "\nPoints:\t\t\tClusters:\nx\t\ty\t\tx\t\ty\n";
    for(auto &i: ergset){
    cout<< i[0]->x << "\t\t" << i[0]->y << "\t\t" << i[1]->x << "\t\t" << i[1] << "\n";
    }
}


// Cluster mit Zufallszahlen initiieren
void KMeans::initClusters(vector<struct Point> &clus){
    for(auto &i: clus){
        i.x = randomFloat(0,11);
        i.y = randomFloat(1,11);
    }
}

// Summiert für jedes Cluster die zugehörigen Punkte auf + Summe der Kluster Skalieren
void KMeans::sumclus(vector<vector<struct Point*> > &ergset, vector<struct Point> &clus){
    int counter=0;
    for(auto &i: clus){
        float sumx = 0;
        float sumy = 0;
        int anzahlP = 0;
        for(auto &j: ergset){
            if(j[1] == &i){
                anzahlP += 1;
                sumx += j[0]->x;
                sumy += j[0]->y;
                // Print zu welchem Kluster ein Punkt gehört
             //  cout<< "\n" << counter << "\tSumme: " << sumx << "\t" << sumy << "\t" << argmin(j[0],clus)->x << "\t" <<  argmin(j[0],clus)->y << "\t" << j[1]->x;
            } 
         //  else {
         //       cout<< "\n" << counter << "\tfalse ";
         //  } 
        }

        // neue Cluster: skaliert durch # an zugehörigen Punkten (wenn keine Punkte dazugehören -> 0/1 = 0)
        if (anzahlP == 0){
            i.x = 0;
            i.y = 0;
        } else {
        i.x = sumx/anzahlP;
        i.y = sumy/anzahlP;
        }
    //    cout<< "\nAnzahl zugehöriger Punkte zu Cluster: " << counter << " : \t" << anzahlP << " \tneue Cluster Werte:\t" << i.x << "\t" << i.y << "\n"; 
    counter+=1;
    }
}

//  Algk-means algorithmus
void KMeans::alg(vector<vector<struct Point*> > &ergset, vector<struct Point> &clus)
{
    bool changec = true;

    // solange sich die Cluster verändern
    while(changec){
        changec = false;

        // für jeden Punkt das Cluster mit dem geringsten Abstand berechnen
        for(auto &i: ergset){
            struct Point* minclus = argmin(i[0], clus);
            if (minclus != i[1]){
               i[1] = minclus;
                changec = true;
            }
        }

        //Cluster neu berechnen (Summe über alle Punkte, die zum gleichen Cluster gehören) + Summe der Kluster Skalieren
        sumclus(ergset, clus);
    }
    // Ergebnisse der Cluster printen
    cout<< "\nDie Ergebniscluster sind:\n";
    printClusters(clus);
  //  printSet(ergset);
}

// Zufallspunkte erstellen
void KMeans::createrndmPoints(vector<vector<struct Point*> > &ergset, int from, int to, vector<struct Point> &points){
    int counter =0;
    for(auto &i: ergset){
    i[0] = &points[counter];
    i[0]->x = randomFloat(from, to);
    i[0]->y = randomFloat(from, to);
    counter += 1;
    }
    vector<struct Point> clus(amount);
    counter = 0;
    for(auto &i: ergset){
        i[1] = &clus[counter];
        i[1]->x = INT_MAX;
        i[1]->y = INT_MAX;
        counter += 1;
    }
}


// Test Punkte erstellen
void KMeans::createPoints(vector<vector<struct Point*> > &ergset, vector<struct Point> &points){
    int counter =0;
    for(auto &i: ergset){
    i[0] = &points[counter];
    counter += 1;
    }
    ergset[0][0]->x = 2;
    ergset[0][0]->y = 1;
    ergset[1][0]->x = 4;
    ergset[1][0]->y = 1;
    ergset[2][0]->x = 1;
    ergset[2][0]->y = 2;
    ergset[3][0]->x = 3;
    ergset[3][0]->y = 2;
    ergset[4][0]->x = 2;
    ergset[4][0]->y = 3;
    ergset[5][0]->x = 3;
    ergset[5][0]->y = 3;
    ergset[6][0]->x = 2;
    ergset[6][0]->y = 4;
    ergset[7][0]->x = 7;
    ergset[7][0]->y = 6;
    ergset[8][0]->x = 8;
    ergset[8][0]->y = 6;
    ergset[9][0]->y = 10;
    ergset[9][0]->y = 6;
    ergset[10][0]->x = 10;
    ergset[10][0]->y = 7;
    ergset[11][0]->x = 7;
    ergset[11][0]->y = 8;
    ergset[12][0]->x = 9;
    ergset[12][0]->y = 8;
    ergset[13][0]->x = 8;
    ergset[13][0]->y = 9;
    ergset[14][0]->x = 11;
    ergset[14][0]->y = 9;

   vector<struct Point> clus(amount);
    counter = 0;
    for(auto &i: ergset){
        i[1] = &clus[counter];
        i[1]->x = INT_MAX;
        i[1]->y = INT_MAX;
        counter += 1;
    }
}


void KMeans::init(int from, int to){

    // Cluster initiieren; Printen
    vector<struct Point> clus(numClus);
    initClusters(clus);
  //  printClusters(clus);

    //Create Set
    vector<struct Point> points(amount);
    vector<vector<struct Point*> > set(amount, vector<struct Point*>(numClus));
   // createPoints(set, points);

    // Zufallspunkte
    createrndmPoints(set, from, to, points);
 //   printSet(set);

    // k-means Algorithmus
    alg(set, clus);

}