#include <iostream>
#include <random>
#include <chrono>
#include "bbtree.hpp"
#include <future>


#define TIMING
 
#ifdef TIMING
#define INIT_TIMER auto start = std::chrono::high_resolution_clock::now();
#define START_TIMER  start = std::chrono::high_resolution_clock::now();
#define STOP_TIMER(name)  std::cout << "RUNTIME of " << name << ": " << \
    std::chrono::duration_cast<std::chrono::milliseconds>( \
            std::chrono::high_resolution_clock::now()-start \
    ).count() << " ms " << std::endl; 
//#define STOP_TIMER auto stop = std::chrono::duration_cast<std::chrono::milliseconds>( \
            std::chrono::high_resolution_clock::now()-start).count();
#else
#define INIT_TIMER
#define START_TIMER
#define STOP_TIMER(name)
#endif

/*int ExecuteThread(int id){
    //current time in msecs
    INIT_TIMER;
    START_TIMER;  

    STOP_TIMER("Thread" << id);
    return id;
}
*/
main() {

    float from = 0;
    float to = 10;
    float dimensions = 2;
    float amount = 10;
    int k = 3;
    int bmax = 4;

    BBTree bbtree(k, bmax);
    bbtree.init(from, to, dimensions, amount);


 /*   // Threads test:
    future<int> th1  = async(&ExecuteThread, 1);
    future<int> th2  = async(&ExecuteThread, 2);
    int x = th1.get();
    int y = th2.get();
    cout<< x << endl; 
    cout<< y << endl; 
    cout<< "Threads executed" << endl;*/

}