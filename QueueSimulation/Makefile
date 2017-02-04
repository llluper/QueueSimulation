simulation: main.o simulation.o queue.o server.o
	g++ -o simulation main.o simulation.o queue.o server.o
main.o: main.cpp
	g++ -c main.cpp
simulation.o: simulation.cpp
	g++ -c simulation.cpp
queue.o: queue.cpp
	g++ -c queue.cpp
server.o: server.cpp
			g++ -c server.cpp
clean:
	rm -f *.o simulation
