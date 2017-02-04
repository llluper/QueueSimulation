//
//  simulation.hpp
//  QueueSimulation
//
//  Created by Mitchell Stanford on 29/1/17.
//  Copyright © 2017 Mitchell Stanford. All rights reserved.
//

#ifndef simulation_hpp
#define simulation_hpp

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "constants.h"
#include "heap.hpp"
#include "queue.hpp"
#include "server.hpp"

using namespace std;

class simulation {
  private:
    int id[MAX];
    int n_busy;
    double Time;
    double queueWaitTime[MAX];
    double idleTimer[MAX];
    double totalIdleTime[MAX];
    int queueCount;
    int queueLength;
    int peopleServed;
    int maxQueueLength;
    double lastServiceComplete, serverIdleTime[MAX], maxQueueWaitTime, averageServiceTime, averageQueueTime, totalQueueWaitTime, allQueue;
    double allServiceTimes;
    //queue qWait[MAX];
    queue *myQueue;
    queue *myQueueSystem;
    //queue myQueueSystem[MAX];
    ifstream ins;

  public:
    simulation();
    ~simulation();
    //INLINE FUNCTIONS

    //void setn_servers(int x) { n_servers = x; };
    bool serversEmpty(int n_busy) { return (n_busy == 0); }

    //void swapAll(int, int);
    //void swapservers(int, int);
    //void siftup(int);
    //void siftdown(int);
    //void siftdowner(int);
    //int findSmallestQueue();
    int n_servers;
    void start();
    void printResults();
    bool openFile();
};






#endif /* simulation_hpp */
