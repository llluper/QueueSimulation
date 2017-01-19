//
//  main.cpp
//  QueueSimulation
//
//  Created by Mitchell Stanford on 19/1/17.
//  Copyright © 2017 Mitchell Stanford. All rights reserved.
//

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "simulation.hpp"

using namespace std;

#define MAX 100
double servers[MAX];
int serversSize = 0;
int id[MAX];
int n_busy;
double Time;
double queueWaitTime[MAX];
double idleTimer[MAX];
double totalIdleTime[MAX];
int n_servers, queueCount, queueLength;
int peopleServed, maxQueueLength;
double lastServiceComplete, serverIdleTime[MAX], maxQueueWaitTime, averageServiceTime, averageQueueTime, totalQueueWaitTime, allQueue;

struct queueWait {
    int id;
    int start;
    int next;
    double wait[MAX];
};

queueWait qWait[MAX];

struct queueStruct {
    double theQueue[MAX];
    int start;
    int next;
};
queueStruct myQueue;
queueStruct myQueueSystem[MAX];

inline bool serversEmpty() { return (n_busy == 0); }

inline bool queueEmpty(queueStruct &myQueue) { return (myQueue.start == myQueue.next); }

void swap(int x, int y) {
    int t = id[x];
    id[x] = id[y];
    id[y] = t;
    double ttl = totalIdleTime[x];
    totalIdleTime[x] = totalIdleTime[y];
    totalIdleTime[y] = ttl;
    double tit = idleTimer[x];
    idleTimer[x] = idleTimer[y];
    idleTimer[y] = tit;
}

void swapservers(int a, int b) {
    double temp = servers[a];
    servers[a] = servers[b];
    servers[b] = temp;
    int t = id[a];
    id[a] = id[b];
    id[b] = t;
    double ttl = totalIdleTime[a];
    totalIdleTime[a] = totalIdleTime[b];
    totalIdleTime[b] = ttl;
    double tit = idleTimer[a];
    idleTimer[a] = idleTimer[b];
    idleTimer[b] = tit;
}

void siftup(int i) {
    int p;
    while (i > 1) {
        p = i /2;
        if (servers[p] > servers[i]) {
            swapservers(p, i);
        }
        i = p;
    }
}

void siftdown(int i) {
    bool skip = false;
    
    while (!skip) {
        int c = (i * 2) + 1;
        bool lexists = (c < n_busy);
        bool rexists = (c + 1 < n_busy);
        
        if (!lexists)
            skip = true;
        if (!lexists && !rexists)
            skip = true;
        
        if (!skip){
            if (servers[c] > servers[c + 1]) c = c + 1;
            if (servers[i] > servers[c]) {
                swapservers(i, c);
            }
        }
        i = c;
    }
}

void enqueue(double elt, queueStruct &myQueue, int id) {
    myQueue.theQueue[myQueue.next] = elt;
    queueWaitTime[myQueue.next] = Time;
    qWait[id].wait[qWait[id].next] = Time;
    qWait[id].next = qWait[id].next + 1;
    myQueue.next = myQueue.next + 1;
    if (maxQueueLength < (myQueue.next - myQueue.start)) {
        maxQueueLength = myQueue.next - myQueue.start;
    }
    if (myQueue.next > MAX-1)
        myQueue.next = 0;
    
}

double dequeue(queueStruct &myQueue, int id) {
    double elt;
    
    if (myQueue.start == myQueue.next) {
        elt = 0;
    } else {
        elt = myQueue.theQueue[myQueue.start];
        qWait[id].wait[qWait[id].start] = Time - qWait[id].wait[qWait[id].start];
        queueWaitTime[myQueue.start] = Time - queueWaitTime[myQueue.start];
        totalQueueWaitTime += queueWaitTime[myQueue.start];
        
        qWait[id].start = qWait[id].start +1;
        myQueue.start = myQueue.start+1;
        queueCount++;
    }
    if (maxQueueWaitTime < queueWaitTime[myQueue.start-1]) {
        maxQueueWaitTime = queueWaitTime[myQueue.start-1];
    }
    if (myQueue.start > MAX-1)
        myQueue.start = 0;
    
    return elt;
}

void siftdowner(int i) {
    bool skip = false;
    
    while (!skip) {
        int c = (i * 2);
        bool lexists = (c <= n_busy);
        bool rexists = (c + 1 <= n_busy);
        
        if (!lexists)
            skip = true;
        if (!lexists && !rexists)
            skip = true;
        
        if (!skip){
            if (servers[c] > servers[c + 1]) c = c + 1;
            if (servers[i] > servers[c]) {
                swapservers(i, c);
            }
        }
        i = c;
    }
}

int findSmallestQueue() {
    int smallestIndex = 1;
    int smallestLength = MAX;
    for (int i = 1; i < n_servers; i++)
        for (int j = 1; j < n_servers; j++)
            if(myQueueSystem[i].next-myQueueSystem[i].start > myQueueSystem[j].next-myQueueSystem[j].start) {
                if (smallestLength > myQueueSystem[j].next-myQueueSystem[j].start) {
                    smallestLength = myQueueSystem[j].next-myQueueSystem[j].start;
                    smallestIndex = j;
                }
            }
    return smallestIndex;
}

int main() {
    cout << "Please enter the filename: ";
    string filename;
    cin >> filename;
    cout << "First run: Single queue from which each server will take the next \ncustomer as soon as the server becomes available." << endl;
    peopleServed = 0;
    maxQueueLength = 0;
    myQueue.start = 0;
    myQueue.next = 0;
    bool finished = false;
    int c = 0;
    ifstream ins;
    ins.open(filename.c_str());
    
    double allServiceTimes = 0;
    totalQueueWaitTime= 0;
    
    maxQueueWaitTime = 0;
    queueCount = 0;
    Time = 0;
    n_busy = 0;
    
    ins >> n_servers;
    n_servers++;
    
    for (int i = 0; i < n_servers; i++) {
        id[i] = i;
        queueWaitTime[i] = 0;
        idleTimer[i] = 0;
        totalIdleTime[i] = 0;
    }
    
    double nextServiceTime = 0.0, serviceTime;
    
    
    ins >> servers[0];
    
    ins >> serviceTime;
    allServiceTimes += serviceTime;
    
    do {
        if (id[0] == 0) {
            if(ins.good()){
                //Process Arrival
                Time = servers[0];
                ins.ignore();
                c = ins.peek();
                if (c >= 48) {
                    ins >> servers[0];
                    ins >> nextServiceTime;
                    allServiceTimes += nextServiceTime;
                    
                    siftdown(0);
                    if (n_busy < n_servers-1) {
                        //idleTimer
                        
                        n_busy = n_busy + 1;
                        servers[n_busy] = Time + serviceTime;
                        totalIdleTime[n_busy] += Time - idleTimer[n_busy];
                        
                        siftup(n_busy);
                        
                    }
                    else {
                        
                        
                        enqueue(serviceTime, myQueue, 0);
                    }
                    serviceTime = nextServiceTime;
                }
            }
        } else {
            //serviceComplete
            
            Time = servers[0];
            
            if (queueEmpty(myQueue)) {
                
                swap(0, n_busy);
                servers[0] = servers[n_busy];
                idleTimer[n_busy] = Time;
                
                n_busy = n_busy - 1;
            }
            else {
                servers[0] = Time + dequeue(myQueue, 0);
            }
            
            siftdown(0);
            peopleServed++;
        }
        
        if (!ins.good()) {
            
            Time = servers[0];
            siftdown(0);
            
            if (n_busy < n_servers-1) {
                n_busy = n_busy + 1;
                servers[n_busy] = Time + serviceTime;
                totalIdleTime[n_busy] += Time - idleTimer[n_busy];
                siftup(n_busy);
            }
            else {
                enqueue(serviceTime, myQueue, 0);
            }
            serviceTime = nextServiceTime;
            
            while (!serversEmpty()) {
                
                Time = servers[1];
                if (queueEmpty(myQueue)) {
                    swap(1, n_busy);
                    servers[1] = servers[n_busy];
                    idleTimer[n_busy] = Time;
                    n_busy = n_busy - 1;
                }
                else {
                    servers[1] = Time + dequeue(myQueue, 0);
                }
                siftdowner(1);
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
                swap(i, j);
    
    if (id[0] != 0)
        swap(0, 1);
    
    for (int i = 1; i < n_servers; i++)
        for (int j = 1; j < n_servers; j++)
            if (id[i] < id[j])
                swap(i, j);
    
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
    ins.close();
    
    //SECOND RUN, Queue for each server
    
    cout << "\nSecond run: Use a queue for each server. Customers will choose the \nserver with the shortest queue on arrival and will not move from queue to queue." << endl;
    
    
    double eachMaxQueueLength[MAX];
    
    double secondTotalQueueWait = 0;
    
    peopleServed = 0;
    maxQueueLength = 0;
    for (int i = 0; i < n_servers; i++) {
        myQueueSystem[i].start = 0;
        myQueueSystem[i].next = 0;
        qWait[i].start = 0;
        qWait[i].next = 0;
    }
    
    finished = false;
    c = 0;
    ins.open(filename.c_str());
    int QID;
    
    allServiceTimes = 0;
    totalQueueWaitTime= 0;
    maxQueueWaitTime = 0;
    queueCount = 0;
    Time = 0;
    n_busy = 0;
    
    ins >> n_servers;
    n_servers++;
    
    for (int i = 0; i < n_servers; i++) {
        id[i] = i;
        queueWaitTime[i] = 0;
        idleTimer[i] = 0;
        totalIdleTime[i] = 0;
        eachMaxQueueLength[i]=0;
        qWait[i].id = i;
    }
    
    
    ins >> servers[0];
    ins >> serviceTime;
    allServiceTimes += serviceTime;
    
    do {
        if (id[0] == 0) {
            if(ins.good()){
                //Process Arrival
                Time = servers[0];
                ins.ignore();
                c = ins.peek();
                if (c >= 48) {
                    ins >> servers[0];
                    ins >> nextServiceTime;
                    allServiceTimes += nextServiceTime;
                    siftdown(0);
                    if (n_busy < n_servers-1) {
                        n_busy = n_busy + 1;
                        servers[n_busy] = Time + serviceTime;
                        totalIdleTime[n_busy] += Time - idleTimer[n_busy];
                        siftup(n_busy);
                    }
                    else {
                        //find smallest queue add to it
                        QID = findSmallestQueue();
                        enqueue(serviceTime, myQueueSystem[QID], QID);
                        if(eachMaxQueueLength[QID] < (myQueueSystem[QID].next - myQueueSystem[QID].start))
                            eachMaxQueueLength[QID] = (myQueueSystem[QID].next - myQueueSystem[QID].start);
                        
                    }
                    serviceTime = nextServiceTime;
                }
            }
        } else {
            //serviceComplete
            Time = servers[0];
            
            if (queueEmpty(myQueueSystem[id[0]])) {
                swap(0, n_busy);
                servers[0] = servers[n_busy];
                idleTimer[n_busy] = Time;
                n_busy = n_busy - 1;
            }
            else {
                servers[0] = Time + dequeue(myQueueSystem[id[0]], id[0]);
                secondTotalQueueWait += qWait[id[0]].wait[qWait[id[0]].start-1];
            }
            siftdown(0);
            peopleServed++;
        }
        
        if (!ins.good()) {
            Time = servers[0];
            siftdown(0);
            
            if (n_busy < n_servers-1) {
                n_busy = n_busy + 1;
                servers[n_busy] = Time + serviceTime;
                totalIdleTime[n_busy] += Time - idleTimer[n_busy];
                siftup(n_busy);
            }
            else {
                QID = findSmallestQueue();
                enqueue(serviceTime, myQueueSystem[QID], QID);
                if(eachMaxQueueLength[QID] < (myQueueSystem[QID].next - myQueueSystem[QID].start))
                    eachMaxQueueLength[QID] = (myQueueSystem[QID].next - myQueueSystem[QID].start);
            }
            serviceTime = nextServiceTime;
            
            while (!serversEmpty()) {
                Time = servers[1];
                
                if (queueEmpty(myQueueSystem[id[1]])) {
                    swap(1, n_busy);
                    servers[1] = servers[n_busy];
                    idleTimer[n_busy] = Time;
                    n_busy = n_busy - 1;
                }
                else {
                    servers[1] = Time + dequeue(myQueueSystem[id[1]], id[1]);
                    secondTotalQueueWait += qWait[id[1]].wait[qWait[id[1]].start-1];
                }
                siftdowner(1);
                peopleServed++;
            }
            finished = true;
        }
    } while(!finished);
    
    for (int i = 1; i < n_servers; i++) {
        totalIdleTime[i] += Time - idleTimer[i];
    }
    
    for (int i = 1; i < n_servers; i++)
        for (int j = 1; j < n_servers; j++)
            if (id[i] < id[j])
                swap(i, j);
    if (id[0] != 0)
        swap(0, 1);
    
    for (int i = 1; i < n_servers; i++)
        for (int j = 1; j < n_servers; j++)
            if (id[i] < id[j])
                swap(i, j);
    
    cout << left << setw(36) << "Number of people served: " << peopleServed << endl;
    cout << left << setw(36) << "Final time: " << setprecision(10) << Time << endl;
    cout << left << setw(36) << "Average service time: " << allServiceTimes/peopleServed << endl;
    cout << left << setw(36) << "Average queue wait time: " << secondTotalQueueWait/peopleServed << endl;
    cout << left << setw(36) << "Max queue wait time: " << maxQueueWaitTime<< endl;
    cout << left << setw(36) << "Average queue length at any second: " << secondTotalQueueWait/Time << endl;
    cout << left << setw(36) << "Max queue length overall: " << maxQueueLength << endl;
    
    for (int i = 1; i < n_servers; i++) {
        cout << left << "Total idle time for server " << id[i] << setw(8) << ": " << totalIdleTime[i] << endl;
        cout << left << "Max queue length for server " << id[i] << setw(7) << ": " << eachMaxQueueLength[i] << endl;
    }
    ins.close();
    
    return 0;
}

