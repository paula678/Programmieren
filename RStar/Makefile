#this target will compile all the files
#main:
#	g++ main.cpp rtree.cpp node.cpp boundingbox.cpp -o all -std=c++11

all:	main.o rtree.o node.o boundingbox.o
	g++ -o all main.o rtree.o node.o boundingbox.o

main.o: main.cpp rtree.hpp
	g++ -c main.cpp -std=c++11

rtree.o: rtree.cpp rtree.hpp
	g++ -c rtree.cpp -std=c++11

node.o: node.cpp node.hpp
	g++ -c node.cpp -std=c++11

boundingbox.o: boundingbox.cpp boundingbox.hpp
	g++ -c boundingbox.cpp -std=c++11

clean:
	rm *o all