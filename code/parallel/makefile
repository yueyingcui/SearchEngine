main : main.o inverted_index.o
	g++ -std=c++0x -pthread -g -o main main.o inverted_index.o 
main.o : main.cpp  
	g++ -std=c++0x -pthread -g -c main.cpp
inverted_index.o : inverted_index.cpp
	g++ -std=c++0x -pthread -g -c inverted_index.cpp
