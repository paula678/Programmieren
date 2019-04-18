

#ifndef _kmeans_hpp_
#define _kmeans_hpp_

#include <vector>

using namespace std;

class KMeans {

    public:

    KMeans (int c, int p);

  //  initPoints(std::vector<Point> p);

    void assignClus(vector<vector<struct Point*> > &ergset, vector<struct Point> &clus);

    float randomFloat(float min, float max);

    void  initClusters(vector<struct Point> &clus);

    float dist(struct Point* p1, struct Point* p2);

    void testDist(struct Point* p1, struct Point* p2);

    struct Point* argmin(struct Point* p, vector<struct Point> &clus);

    void testArgmin(struct Point* p, vector<struct Point> &clus);
  
    void printClusters(vector<struct Point> &clus);

    void printSet(vector<vector<struct Point*> > &ergarr);

    void sumclus(vector<vector<struct Point*> > &ergset, vector<struct Point> &clu);

    void alg(vector<vector<struct Point*> > &ergset, vector<struct Point> &clus);

    void createrndmPoints(vector<vector<struct Point*> > &ergset, int from, int to, vector<struct Point> &points);

    void createPoints(vector<vector<struct Point*> > &ergset, vector<struct Point> &points);

    void init(int from, int to);

};

#endif