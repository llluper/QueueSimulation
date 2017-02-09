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
    queue *myQueue;
    queue *myQueueSystem;
    ifstream ins;
    heap<server> *theHeap;
    double nextServiceTime;
    double serviceTime;
  public:
    simulation();
    ~simulation();
    bool serversEmpty(int n_busy) { return (n_busy == 0); }
    int n_servers;
    void start();
    void printResults();
    bool openFile();
    void processArrival();
    void serviceComplete();
};






#endif /* simulation_hpp */
