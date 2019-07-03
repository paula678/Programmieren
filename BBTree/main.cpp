#include <iostream>
#include <random>
#include <chrono>
#include "bbtree.hpp"
/*
#define TIMING
 
#ifdef TIMING
#define INIT_TIMER auto start = std::chrono::high_resolution_clock::now();
#define START_TIMER  start = std::chrono::high_resolution_clock::now();
//#define STOP_TIMER(name)  std::cout << "RUNTIME of " << name << ": " << \
    std::chrono::duration_cast<std::chrono::milliseconds>( \
            std::chrono::high_resolution_clock::now()-start \
    ).count() << " ms " << std::endl; 
#define STOP_TIMER auto stop = std::chrono::duration_cast<std::chrono::milliseconds>( \
            std::chrono::high_resolution_clock::now()-start).count();
#else
#define INIT_TIMER
#define START_TIMER
#define STOP_TIMER(name)
#endif
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



}
