
#ifndef _kmeans_hpp_
#define _kmeans_hpp_

#include <vector>

using namespace std;

class KMeans {

    public:

    KMeans (int c, int p);

    void assignClus(vector<vector<struct Point*> > &ergset, vector<struct Point> &clus);

    float randomFloat(float min, float max);

    void  initClusters(vector<vector<float>> &clus);

    float dist(vector<float> &p1, vector<float> &p2);

    void testDist(vector<float> &p1, vector<float> &p2);

    vector<float> * argmin(vector<float> &p, vector<vector<float> > &clus);

    void testArgmin(vector<float> &p, vector<vector<float> > &clus);
  
    void printPoints(vector<vector<float>> &clus);

    void printSet(vector<vector<float> * > &ergarr);

    void initSet(vector<vector<float> * > &ergset);

    void sumclus(vector<vector<float> * > &ergset, vector<vector<float> > &clus, vector<vector<float> > &points);

    void alg(vector<vector<float> * > &ergset, vector<vector<float> > &clus, vector<vector<float> > &points);

    void createrndmPoints(int from, int to, vector<vector<float> > &points);

    void init(int from, int to, int dimension);

};

#endif