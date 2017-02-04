//
//  constants.h
//  QueueSimulation
//
//  Created by Mitchell Stanford on 1/2/17.
//  Copyright © 2017 Mitchell Stanford. All rights reserved.
//

#ifndef constants_h
#define constants_h

enum Constants { MAX = 1000 };

struct QueueStatistics {
	int peopleServed;
	double Time;
	double averageServiceTime;
	double averageQueueWaitTime;
	double maxQueueWaitTime;
	double averageQueueLength;
	int maxQueueLength;
	double serverIdleTime;
};

#endif /* constants_h */
