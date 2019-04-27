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

KMeans::KMeans(int c, int p) {
    numClus = c;
    amount = p;
}


//random Float erstellen
float KMeans::randomFloat(float min, float max) {
    return (min + 1) + (((float) rand()) / (float) RAND_MAX) * (max - (min + 1)); 
}


// ordnet Punkte ihren nächsten Clustern zu
void KMeans::assignClus(vector<vector<struct Point*> > &ergset, vector<struct Point> &clus){
    for(auto &i: ergset){
   //   i[1] = argmin(i[0], clus);
    }
}


// Distanz zwischen zwei Punkten
float KMeans::dist(vector<float> &p1, vector<float> &p2){
    int counter = 0;
    float add = 0;
    for(const auto &i : p1){
        add += pow(i - p2[counter], 2);
        counter += 1;
    }
    return sqrtf(add);
 }

 // Test dist
void KMeans::testDist(vector<float> &p1, vector<float> &p2){
    float abst = dist(p1, p2);
    std::cout<< "\nAbstand zwischen zwei Punkten:\t" << abst << "\n";
}


// berechnet für jeden Punkt den Abstand zu jedem Zentrum -> return: Zentrum mit kleinstem Abstand
vector<float> * KMeans::argmin(vector<float> &p, vector<vector<float> > &clus){
    vector<float> * zentrum;
    float min = INT_MAX;
    for(auto &i: clus){
        float abst = dist(p, i);
        if(abst < min){
            min = abst;
            zentrum = &i;
        }
    }
    return zentrum;
}


    // Test argmin
void KMeans::testArgmin(vector<float> &p, vector<vector<float> > &clus){
    cout<< "\n Test Argmin: \n";
    for(auto &i : clus){
        float d = dist(p, i);
        cout<< "Abstand:\t" << d << "\n";
    }
}


// Printed Cluster
void KMeans::printPoints(vector<vector<float>> &clus){
    cout<< "\nPoints:\n";
    for(const auto &i: clus){
        for (const auto &j: i)
        cout<< j << "\t";
        cout<< "\n";
    }
}


// Printed ein set von Punkten
void KMeans::printSet(vector<vector<float> * > &ergset){
    cout<< "\n\tClusters:\n";
    int counter = 0;
    for(auto &i: ergset){
        cout << counter << ":\t";
            for(auto &j : *i)
            cout << j << "\t";
        cout<< "\n"; 
        counter += 1;
    }
}


// Cluster mit Zufallszahlen initiieren
void KMeans::initClusters(vector<vector<float>> &clus){
    for(auto &i: clus){
        for(auto &j: i){
            j = randomFloat(0, 11);
        }
    }
}


// Summiert für jedes Cluster die zugehörigen Punkte auf + Summe der Kluster Skalieren
void KMeans::sumclus(vector<vector<float> * > &ergset, vector<vector<float> > &clus, vector<vector<float> > &points){
    for(auto &i: clus){
        int anzahlP = 0;
        vector<float> sum (clus[0].size());
        for(int j = 0; j < points.size(); j++){
            if(ergset[j] == &i){
                for(int k = 0; k < ergset[0]->size(); k++){
                    sum[k] += points[j][k];
                }
                anzahlP += 1;
            } 
        }

        // neue Cluster: skaliert durch # an zugehörigen Punkten (wenn keine Punkte dazugehören -> 0/1 = 0)
        for(int k = 0; k < i.size(); k++){
            i[k] = sum[k]/anzahlP;
        } 
    }
}

//  Algk-means algorithmus
void KMeans::alg(vector<vector<float> * > &ergset, vector<vector<float> > &clus, vector<vector<float> > &points){
    bool changec = true;

    // solange sich die Cluster verändern
    while(changec){
        changec = false;

        // für jeden Punkt das Cluster mit dem geringsten Abstand berechnen
        int counter = 0;
        for(auto &i: ergset){
            vector<float> * minclus = argmin(points[counter], clus);
            if (minclus != i){
               i = minclus;
                changec = true;
            }
            counter += 1;
        }
        if (changec)

        //Cluster neu berechnen (Summe über alle Punkte, die zum gleichen Cluster gehören) + Summe der Kluster Skalieren
        sumclus(ergset, clus, points);
    }
    // Ergebnisse der Cluster printen
    cout<< "\nDie Ergebniscluster sind:\n";
    printPoints(clus);
  //  printSet(ergset);
}


// Zufallspunkte erstellen
void KMeans::createrndmPoints(int from, int to, vector<vector<float> > &points){
    int counter =0;
    // Zufallspunkte erstellen
    for(auto &i: points){
        for(auto &j: i)
        j = randomFloat(from, to);
    }
}


void KMeans::initSet(vector<vector<float> * > &ergset){
    vector<float> init = {0, 0};
    for(auto &i : ergset)
    i = &init;
   // printSet(ergset);
}


void KMeans::init(int from, int to, int dimension){

    // Cluster initiieren
    vector<vector<float>> clus (numClus, vector<float> (dimension));
    initClusters(clus);
  //  printPoints(clus);

    // Create points
    vector<vector<float>> points (amount, vector<float> (dimension));

    // Create clusterset to points
    vector<vector<float> * > ergset (amount);

    // Zufallspunkte
    createrndmPoints(from, to, points);
    initSet(ergset);
  //  printPoints(points);

    // k-means Algorithmus
    alg(ergset, clus, points);

}
