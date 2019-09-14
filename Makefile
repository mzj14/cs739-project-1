# Makefile
# Requires that the object definitions be in wl.h and wl.cpp

CXX = g++
CXXFLAGS = -O2 -g -Wall -std=c++11 
OBJECTS = main.o lib739kv.o

# During debugging you may want to used the compiler flags listed below
#CXXFLAGS =      -g -Wall

all: main.o lib739kv.o
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o output

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

lib739kv.o: lib739kv.cpp lib739kv.h
	$(CXX) $(CXXFLAGS) -c lib739kv.cpp

clean:
	rm -f core *.o output

