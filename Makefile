CPPFLAGS = --std=c++11 -Wall -g

all: *.cpp
	g++ $(CPPFLAGS) *.cpp

%.o: %.cpp
	g++ $(CPPFLAGS) -c $<
