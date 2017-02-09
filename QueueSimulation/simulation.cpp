//
//  simulation.cpp
//  QueueSimulation
//
//  Created by Mitchell Stanford on 29/1/17.
//  Copyright © 2017 Mitchell Stanford. All rights reserved.
//

#include "simulation.hpp"

simulation::simulation() {
    peopleServed = 0;
    maxQueueLength = 0;
    totalQueueWaitTime= 0;
    maxQueueWaitTime = 0;
    queueCount = 0;
    Time = 0;
    n_busy = 0;
    allServiceTimes = 0;
    nextServiceTime = 0.0;
    serviceTime = 0.0;

    myQueue = new queue(queueCount++);
}

simulation::~simulation() {
    ins.close();
}


void simulation::printResults() {
    cout << left << setw(36) << "Number of people served: " << peopleServed << endl;
    cout << left << setw(36) << "Final time: " << setprecision(10) << Time << endl;
    cout << left << setw(36) << "Average service time: " << allServiceTimes/peopleServed << endl;
    cout << left << setw(36) << "Average queue wait time: " << totalQueueWaitTime/peopleServed << endl;
    cout << left << setw(36) << "Max queue wait time: " << maxQueueWaitTime<< endl;
    cout << left << setw(36) << "Average queue length at any second: " << totalQueueWaitTime/Time << endl;
    cout << left << setw(36) << "Max length of queue: " << maxQueueLength << endl;
    for (int i = 1; i < n_servers; i++) {
        cout << left << "Total idle time for server " << id[i] << setw(8) << ": " << totalIdleTime[i] << endl;
    }
}

bool simulation::openFile() {
    bool fine = false;
    cout << "Please enter the filename: ";
    string filename;
    cin >> filename;
    cout << "First run: Single queue from which each server will take the next \ncustomer as soon as the server becomes available.\n";

    ins.open(filename.c_str());
    if (ins.good()) {
        fine = true;
        ins >> n_servers;
        n_servers++;
    }

    return fine;
}

void simulation::processArrival() {

  Time = theHeap->servers[0].getData();
  ins >> theHeap->servers[0].data;
  ins >> nextServiceTime;
  allServiceTimes += nextServiceTime;
  theHeap->siftdown(0);

  if (n_busy < n_servers-1) {
      n_busy += 1;
      theHeap->servers[n_busy].setData(Time + serviceTime);
      totalIdleTime[n_busy] += Time - idleTimer[n_busy];
      theHeap->siftup(n_busy);
  }
  else {
      myQueue->enqueue(serviceTime, 0);
  }
  serviceTime = nextServiceTime;
}

void simulation::serviceComplete() {
  Time = theHeap->servers[0].getData();
  if (myQueue->queueEmpty()) {
      theHeap->swap(theHeap->servers[0],theHeap->servers[n_busy+1]);
      theHeap->servers[0].data = theHeap->servers[n_busy+1].data;
      idleTimer[n_busy] = Time;
      n_busy = n_busy - 1;
  }
  else {
      theHeap->servers[0].data = Time + myQueue->dequeue(0);
  }

  theHeap->siftdown(0);
  peopleServed++;
}

void simulation::start() {
    bool fine = openFile();

    if (!fine)
        cout << "Error opening file" << endl;
    else {
        bool finished = false;
        int c = 0;

				theHeap = new heap<server>(n_servers);

        int id[MAX];

        for (int i = 0; i < n_servers; i++)
            id[i] = i;

				double arrive;
        ins >> arrive;
				theHeap->servers[0].setData(arrive);

        ins >> serviceTime;
        allServiceTimes += serviceTime;

        do {
          if(ins.good()) {
            if (theHeap->servers[0].id == 0) {
                processArrival();
            } else {
                serviceComplete();
            }
          }
            else if (!ins.good()) {
                Time = theHeap->servers[0].getData();
                theHeap->siftdown(0);

                if (n_busy < n_servers-1) {
                    n_busy = n_busy + 1;
                    theHeap->servers[n_busy].data = Time + serviceTime;
                    theHeap->siftup(n_busy);
                }
                else {
                    myQueue->enqueue(serviceTime, 0);
                }
                serviceTime = nextServiceTime;
                theHeap->siftdown(0);

                while ((n_busy)) {


                    Time = theHeap->servers[1].data;

                    theHeap->print();

                    if (myQueue->queueEmpty()) {
												theHeap->swap(theHeap->servers[1],theHeap->servers[n_busy]);
                        theHeap->servers[1].data = theHeap->servers[n_busy].data;
                        n_busy = n_busy - 1;
                    }
                    else {
                        theHeap->servers[1].data = Time + myQueue->dequeue(0);
                    }
                    theHeap->print();

                    theHeap->siftdowner(1);
                    theHeap->print();

                    peopleServed++;


                }
                finished = true;
            }

        } while(!finished);

        for (int i = 1; i < n_servers; i++)
            totalIdleTime[i] += Time - idleTimer[i];

        for (int i = 1; i < n_servers; i++)
            for (int j = 1; j < n_servers; j++)
                if (id[i] < id[j])
                    swapAll(i, j);

        if (id[0] != 0)
            swapAll(0, 1);

        for (int i = 1; i < n_servers; i++)
            for (int j = 1; j < n_servers; j++)
                if (id[i] < id[j])
                    swapAll(i, j);

        printResults();

        ins.close();
    }
}
