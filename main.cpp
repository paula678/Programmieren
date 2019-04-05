#include <iostream>
#include <chrono>
#include "kmeans.hpp"

#define TIMING
 
#ifdef TIMING
#define INIT_TIMER auto start = std::chrono::high_resolution_clock::now();
#define START_TIMER  start = std::chrono::high_resolution_clock::now();
#define STOP_TIMER(name)  std::cout << "RUNTIME of " << name << ": " << \
    std::chrono::duration_cast<std::chrono::milliseconds>( \
            std::chrono::high_resolution_clock::now()-start \
    ).count() << " ms " << std::endl; 
#else
#define INIT_TIMER
#define START_TIMER
#define STOP_TIMER(name)
#endif



main() {

    //Start the Timer
    INIT_TIMER
    START_TIMER
    
    // KMeans initialisieren
    int numberClus = 2;
    int amountPoints = 10000000;
    KMeans kmeans(numberClus, amountPoints);
    // Range der Punkte
    int from = 0;
    int to = 11;
    kmeans.init(from, to);

    //Stop the Timer
    STOP_TIMER("KMeans algorithm")

}