#include <iostream>
#include <vector>

using namespace std;

void ok(vector<vector<float> * > &ergset){
    cout<<"hiii" << ergset[0]->at(1) << "\n";
}


// Printed ein set von Punkten
void printSet(vector<vector<float> * > &ergset){
    cout<< "\n\tClusters:\n";
    int counter = 0;
    for(const auto &i: ergset){
        cout << counter << ":\t";
            for(const auto &j : *i)
            cout << j << "\t";
        cout<< "\n"; 
        counter += 1;
    }
    ok(ergset);
}

void init(vector<vector<float> * > &ergset, vector<float> init){
    for(auto &i : ergset)
    i = &init;
    printSet(ergset);
}


main(){

vector<float> ini = {0, 1};
vector<vector<float> * > ergset (3);
init(ergset, ini);
    cout<<"hiii" << ergset.size() << "\n";


//float sum = &(ergset[0]);
vector<float> sum (ergset.size());
cout << sum[1];

}


