#this target will compile all the files
all:	main.o kmeans.o
	g++ -o all main.o kmeans.o

main.o: main.cpp kmeans.hpp PerfEvent.hpp
	g++ -c main.cpp -std=c++14

kmeans.o: kmeans.cpp kmeans.hpp
	g++ -c kmeans.cpp -std=c++14

clean:
	rm *o all