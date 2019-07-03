#this target will compile all the files
all:	main.o bbtree.o bb.o
	g++ -o all main.o bbtree.o bb.o

main.o: main.cpp bbtree.hpp
	g++ -c main.cpp -std=c++14

bbtree.o: bbtree.cpp bbtree.hpp
	g++ -c bbtree.cpp -std=c++14

bb.o: bb.cpp bb.hpp
	g++ -c bb.cpp -std=c++14

clean:
	rm *o all