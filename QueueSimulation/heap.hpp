//
//  heap.hpp
//  QueueSimulation
//
//  Created by Mitchell Stanford on 29/1/17.
//  Copyright © 2017 Mitchell Stanford. All rights reserved.
//

#ifndef heap_hpp
#define heap_hpp

#include "constants.h"
#include "server.hpp"
//#include "simulation.hpp"
#include <iostream>

template <class T>
class heap {
public:
	int size;
	T *servers;
	heap(int);
	~heap();
	void siftup(int);
	void siftdown(int);
	void siftdowner(int);
	void swap(T&, T&);
	int n_busy;
};

template <class T>
heap<T>::heap(int size) {
	this->size = size;
	servers = new server[size];
	n_busy = 0;
}

template <class T>
heap<T>::~heap() {
	delete [] servers;
}

template <class T>
void heap<T>::siftup(int i) {
    int p;
    while (i > 1) {
        p = i /2;
        if (servers[p].data > servers[i].data) {
            swap(servers[p], servers[i]);
        }
        i = p;
    }
}

template <class T>
void heap<T>::siftdown(int i) {
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
					if (servers[c].data > servers[c + 1].data) c = c + 1;
					if (servers[i].data > servers[c].data) {
							swap(servers[i], servers[c]);
            }
        }
        i = c;
    }
}

template <class T>
void heap<T>::siftdowner(int i) {
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
            if (servers[c].data > servers[c + 1].data) c = c + 1;
            if (servers[i].data > servers[c].data) {
                swap(servers[i], servers[c]);
            }
        }
        i = c;
    }
}

template <class T>
void heap<T>::swap(T &x, T &y) {
    T t = x;
		x = y;
		y = t;
}

#endif /* heap_hpp */
