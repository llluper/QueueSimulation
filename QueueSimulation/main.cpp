//
//  main.cpp
//  QueueSimulation
//
//  Created by Mitchell Stanford on 19/1/17.
//  Copyright © 2017 Mitchell Stanford. All rights reserved.
//

#include "simulation.hpp"

int main() {
    simulation *mySim = new simulation();
    mySim->start();
    delete mySim;

    return 0;
}
