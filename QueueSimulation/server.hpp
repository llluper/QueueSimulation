//
//  queue.hpp
//  QueueSimulation
//
//  Created by Mitchell Stanford on 1/2/17.
//  Copyright © 2017 Mitchell Stanford. All rights reserved.
//

#ifndef server_hpp
#define server_hpp

#include "constants.h"

class server {
protected:
	static int nextID;
  private:
		bool state;
		double idleTime;
		double totalIdleTime;
  public:
    server();
    ~server();
		double data;
		int id;
		bool getState() { return state; }
		void setState(bool state) { this->state = state; }
		double getData() { return data; }
		void setData(double data) { this->data = data; }
		server& operator=(const server &);
};

//int server::nextID = 1;

#endif /* server_hpp */
