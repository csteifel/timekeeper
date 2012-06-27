all: main.cpp
	g++ -Wall -std=c++11 main.cpp -o timekeeper -pthread

debug: main.cpp
	g++ -g -Wall -std=c++11 main.cpp -o timekeeper -pthread
