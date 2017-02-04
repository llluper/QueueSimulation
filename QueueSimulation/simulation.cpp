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

    myQueue = new queue(queueCount++);
    //qWait = new queue(queueCount);
}

simulation::~simulation() {
    ins.close();
}


void simulation::printResults() {
    //PRINT RESULTS OF THE FIRST SIMULATION
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
    //FILENAME INPUT
    cout << "Please enter the filename: ";
    string filename;
    filename = "test2.txt";
    //cin >> filename;
    cout << "First run: Single queue from which each server will take the next \ncustomer as soon as the server becomes available.\n";

    //OPEN FILE
    //ifstream ins;
    ins.open(filename.c_str());
    if (ins.good()) {
        fine = true;
        ins >> n_servers;
        n_servers++;
    }
    return fine;
}

void simulation::start() {
    bool fine = openFile();

    if (!fine)
        cout << fine << " X " << n_servers << endl;
    else {
        bool finished = false;
        int c = 0;

				heap<server> *theHeap = new heap<server>(n_servers);


        //CONTINUE INITIALIZING
        for (int i = 0; i < n_servers; i++) {
            id[i] = i;
            queueWaitTime[i] = 0;
            idleTimer[i] = 0;
            totalIdleTime[i] = 0;
						cout << "SERVERS ID: " << theHeap->servers[i].id << endl;
        }


        double nextServiceTime = 0.0, serviceTime = 0.0;
        //
				double arrive;
        ins >> arrive;
				theHeap->servers->setData(arrive);

        ins >> serviceTime;
        allServiceTimes += serviceTime;

        cout << theHeap->servers[0].data << " " << serviceTime << endl;

        do {
					//cout << "ID: " << id[0] << endl;
            if (theHeap->servers[0].id == 0) {
                if(ins.good()){

                    //Process Arrival
                    Time = theHeap->servers[0].data;
                    ins.ignore();
                    c = ins.peek();
                    if (c >= 48) {

                        ins >> theHeap->servers[0].data;
                        ins >> nextServiceTime;
                        allServiceTimes += nextServiceTime;
                        cout << theHeap->servers[0].data << " LOOP " << nextServiceTime << endl;

                        theHeap->siftdown(0);
                        if (n_busy < n_servers-1) {
                            //idleTimer
cout << "ONE" << endl;
                            n_busy += 1;
                            theHeap->servers[n_busy].data = Time + serviceTime;
                            totalIdleTime[n_busy] += Time - idleTimer[n_busy];

                            theHeap->siftup(n_busy);

                        }
                        else {
													cout << "TWOw" << endl;

                            // queueWaitTime[myQueue->next] = Time;
                            myQueue->enqueue(serviceTime, 0);
                        }
                        serviceTime = nextServiceTime;
                    }
                }
            } else {
                //serviceComplete
                cout << "service complete" << peopleServed << endl;

                Time = theHeap->servers[0].getData();

                if (myQueue->queueEmpty()) {
									////swapAll(0, n_busy);
									theHeap->swap(theHeap->servers[0],theHeap->servers[n_busy]);
                    theHeap->servers[0].data = theHeap->servers[n_busy].data;
                    idleTimer[n_busy] = Time;

                    n_busy = n_busy - 1;
                }
                else {
                    theHeap->servers[0].data = Time + myQueue->dequeue(0);
                }

                theHeap->siftdown(0);
                peopleServed++;
            }

            if (!ins.good()) {
							cout << "FILE DONE" << endl;
                Time = theHeap->servers[0].getData();
                theHeap->siftdown(0);

                if (n_busy < n_servers-1) {
                    n_busy = n_busy + 1;
                    theHeap->servers[n_busy].data = Time + serviceTime;
                    totalIdleTime[n_busy] += Time - idleTimer[n_busy];
                    theHeap->siftup(n_busy);
                }
                else {
                    //queueWaitTime[myQueue->next] = Time;
                    myQueue->enqueue(serviceTime, 0);

                }
                serviceTime = nextServiceTime;

								cout << "NBUSYYYY : " << n_busy << endl;
                while ((n_busy)) {
										cout << "Time : " << Time << endl;
                    Time = theHeap->servers[1].data;
                    if (myQueue->queueEmpty()) {
                        ////swapAll(1, n_busy);
												theHeap->swap(theHeap->servers[1],theHeap->servers[n_busy]);
                        theHeap->servers[1].data = theHeap->servers[n_busy].data;
                        ////idleTimer[n_busy] = Time;
                        n_busy = n_busy - 1;
                    }
                    else {
                        theHeap->servers[1].data = Time + myQueue->dequeue(0);
                    }
                    theHeap->siftdowner(1);
                    peopleServed++;
                }
                finished = true;
            }

        } while(!finished);

				/*//////CHECK IDLE TIMER ETC IN T?HE CLASS SERVER
				cout << "PEOPLE SERVED : " << peopleServed << endl;
        //
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
*/
        printResults();

        ins.close();
    }
}


//myQueueSystem = new queue[n_servers];
//SECOND RUN, Queue for each server
/*
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
										queueWaitTime[next] = Time; //NEW
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
						swapAll(0, n_busy);
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

						queueWaitTime[next] = Time;//NEW
						enqueue(serviceTime, myQueueSystem[QID], QID);
						if(eachMaxQueueLength[QID] < (myQueueSystem[QID].next - myQueueSystem[QID].start))
								eachMaxQueueLength[QID] = (myQueueSystem[QID].next - myQueueSystem[QID].start);
				}
				serviceTime = nextServiceTime;

				while (!serversEmpty(n_busy)) {
						Time = servers[1];

						if (queueEmpty(myQueueSystem[id[1]])) {
								swapAll(1, n_busy);
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

//
for (int i = 1; i < n_servers; i++) {
		totalIdleTime[i] += Time - idleTimer[i];
}

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




//PRINT RESULTS OF THE SECOND SIMULATION
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
 */


/*
template <class T>
void simulation::swap(T set[], int x, int y) {
    T t = set[x];
    set[x] = set[y];
    set[y] = t;
}

void simulation::swapAll(int x, int y) {
    swap(id, x, y);
    swap(totalIdleTime, x, y);
    swap(idleTimer, x, y);
}*/
//void simulation::swapservers(int x, int y) {
/*
		double temp = servers[x];
    servers[x] = servers[y];
    servers[y] = temp;
    int t = id[x];
    id[x] = id[y];
    id[y] = t;
    double ttl = totalIdleTime[x];
    totalIdleTime[x] = totalIdleTime[y];
    totalIdleTime[y] = ttl;
    double tit = idleTimer[x];
    idleTimer[x] = idleTimer[y];
    idleTimer[y] = tit;
*//*
    swap(servers, x, y);
    swap(id, x, y);
    swap(totalIdleTime, x, y);
    swap(idleTimer, x, y);
}*/
/*
void simulation::siftup(int i) {
    int p;
    while (i > 1) {
        p = i /2;
        if (servers[p] > servers[i]) {
            swapservers(p, i);
        }
        i = p;
    }
}

void simulation::siftdown(int i) {
    bool skip = false;

    while (!skip) {
        int c = (i * 2) + 1;
        bool lexists = (c <= n_busy);
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

void simulation::siftdowner(int i) {
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
*/




/*

int simulation::findSmallestQueue() {
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
}*/
