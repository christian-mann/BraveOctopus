CPPFLAGS = --std=c++11 -Wall -g -stdlib=libc++ -Wall

all: run.out gen.out

run.out: *.cpp
	g++ $(CPPFLAGS) main.cpp -o run.out

gen.out: *.cpp
	g++ $(CPPFLAGS) main-generator.cpp -o gen.out

%.o: %.cpp
	g++ $(CPPFLAGS) -c $<
