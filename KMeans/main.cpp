#include <iostream>
#include <random>
#include <chrono>
#include "kmeans.hpp"
#include "PerfEvent.hpp"

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



main() {
    
    // KMeans initialisieren
    cout<< "Geben Sie die höchste Anzahl an Clustern ein:";
    int numberClus;
    cin>> numberClus;
   // numberClus = 2;

    cout<< "Geben Sie die höchste Anzahl an Punkten ein:";
    int amountPoints;
    cin>> amountPoints;
  // amountPoints = 1000;

    //Anzahl an Durchläufen für Zeit messen
    int iterations;
    cout<< "Geben Sie die Anzahl an Durchläufen ein:";
    cin>> iterations;
 // iterations = 1;
    
    int min = INT16_MAX;
    int max = INT16_MIN;
    int avg = 0;

    for(int i = 0; i < iterations; i++){
         //Start the Timer
        INIT_TIMER
        START_TIMER

        KMeans kmeans(numberClus, amountPoints);

        // Range der Punkte
        int from = 0;
        int to = 11;
        int dimension = 2;
        kmeans.init(from, to, dimension);

        //Stop the Timer
        STOP_TIMER
        avg += stop;
        if(stop < min)
            min = stop;
        if(stop > max)
            max = stop;
    }
    avg = avg/iterations;
    cout<< "\nmin time: " << min << " ms\tmax time: " << max << " ms\tavg time: " << avg << " ms" << endl; 


}

// Tupel - Zeit/s
// # Cluster Zeit/s
// lin/log hochgehen - jeden Wert 1 - 10 mal messen