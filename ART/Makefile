#this target will compile all the files
main:	main.o art.o node.o
	g++ -o main main.o art.o node.o

main.o: main.cpp 
	g++ -c main.cpp -std=c++14

art.o: art.cpp 
	g++ -c art.cpp -std=c++14

node.o: node.cpp 
	g++ -c node.cpp -std=c++14

clean:
	rm *o main