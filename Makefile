CPPFLAGS = --std=c++11 -Wall

all: a.out

a.out: *.cpp
	g++ $(CPPFLAGS) main.cpp

%.o: %.cpp
	g++ $(CPPFLAGS) -c $<
