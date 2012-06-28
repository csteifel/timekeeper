all: main.cpp
	g++ -Wall -std=c++11 `pkg-config gtkmm-3.0 --cflags --libs` main.cpp -o timekeeper -pthread

debug: main.cpp
	g++ -g -Wall -std=c++11 `pkg-config gtkmm-3.0 --cflags --libs` main.cpp -o timekeeper -pthread
