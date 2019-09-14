# Makefile
# Requires that the object definitions be in wl.h and wl.cpp

CXX = g++
CXXFLAGS =  -O2 -g -Wall -std=c++11

# During debugging you may want to used the compiler flags listed below
#CXXFLAGS =      -g -Wall

all: main

main: main.cpp 
	$(CXX) $(CXXFLAGS) main.cpp -o $@

clean:
	rm -f core *.o main 

