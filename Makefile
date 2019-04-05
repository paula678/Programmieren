#this target will compile all the files
all:	main.o kmeans.o	
	g++ -o all main.o kmeans.o

main.o: main.cpp kmeans.hpp
	g++ -c main.cpp

kmeans.o: kmeans.cpp kmeans.hpp
	g++ -c kmeans.cpp

clean:
	rm *o all