main : main.o inverted_index.o
	g++ -std=c++0x -g -o main main.o inverted_index.o
main.o : main.cpp  
	g++ -std=c++0x -g -c main.cpp
inverted_index.o : inverted_index.cpp
	g++ -std=c++0x -g -c inverted_index.cpp
