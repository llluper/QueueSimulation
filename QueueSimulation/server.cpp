//
//  queue.cpp
//  QueueSimulation
//
//  Created by Mitchell Stanford on 1/2/17.
//  Copyright © 2017 Mitchell Stanford. All rights reserved.
//

#include "server.hpp"

int server::nextID = 1;

server::server() {
	state = false;
	idleTime = 0.0;
	totalIdleTime = 0.0;
	data = 0.0;
	id = nextID++;
}

server::~server() {

}


server& server::operator=(const server &x) {
	state = x.state;
	data = x.data;
	idleTime = x.idleTime;
	totalIdleTime = x.totalIdleTime;
	return *this;
}
