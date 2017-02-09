//
//  queue.cpp
//  QueueSimulation
//
//  Created by Mitchell Stanford on 1/2/17.
//  Copyright © 2017 Mitchell Stanford. All rights reserved.
//

#include "queue.hpp"

queue::queue(int id) {
    this->id = id;
    start = 0;
    next = 0;
}

void queue::enqueue(double elt, int id) {
    this->customers[this->next] = elt;
    this->next = this->next + 1;
    if (this->next > MAX-1)
        this->next = 0;
}

double queue::dequeue(int id) {
    double elt;
    if (this->start == this->next) {
        elt = 0;
    } else {
        elt = this->customers[this->start];
        this->start = this->start+1;
    }

    if (this->start > MAX-1)
        this->start = 0;

    return elt;
}
