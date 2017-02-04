//
//  queue.hpp
//  QueueSimulation
//
//  Created by Mitchell Stanford on 1/2/17.
//  Copyright © 2017 Mitchell Stanford. All rights reserved.
//

#ifndef queue_hpp
#define queue_hpp

#include "constants.h"

class queue {
  private:
		double customers[MAX];
    int id;
    int start;
    int next;
    //double wait[MAX];
  public:
    queue(int);
    ~queue();
    void enqueue(double, int);
    double dequeue(int);
    bool queueEmpty() { return (this->start == this->next); }
};

#endif /* queue_hpp */
