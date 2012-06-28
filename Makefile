all: main.cpp
	g++ -Wall -std=c++11 main.cpp -o timekeeper -pthread `pkg-config gtkmm-3.0 --cflags --libs`

debug: main.cpp
	g++ -g -Wall -std=c++11 main.cpp -o timekeeper -pthread `pkg-config gtkmm-3.0 --cflags --libs`
