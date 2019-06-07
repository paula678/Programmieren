#this target will compile all the files
#main:
#	g++ main.cpp rtree.cpp node.cpp boundingbox.cpp -o ma -std=c++11

all:	main.o
	g++ -o all main.o

main.o: main.cpp rtree.hpp
	g++ -c main.cpp -std=c++11

rtree.o: rtree.o rtree.hpp
	g++ -c rtree.cpp -std=c++11

node.o: node.o node.hpp
	g++ -c node.cpp -std=c++11

boundingbox.o: boundingbox.o boundingbox.hpp
	g++ -c boundingbox.cpp -std=c++11

clean:
	rm *o main