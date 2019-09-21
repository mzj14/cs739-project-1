# Makefile
# Requires that the object definitions be in wl.h and wl.cpp

# CXX = g++
# CXXFLAGS = -O2 -g -Wall -std=c++11 
CXX = gcc
CXXLINK = -lcpprest -lboost_system -lcrypto -lssl 

# During debugging you may want to used the compiler flags listed below
#CXXFLAGS =      -g -Wall

all: 
	make lib739kv.so
	make main.o

main.o: main.c
	export LD_LIBRARY_PATH=$(pwd)
	$(CXX) main.c -L. -l :lib739kv.so -o output 

lib739kv.so: 739kv.h 739kv.cpp 
	rm -f *.so
	g++ $(CXXFLAGS) 739kv.cpp -fPIC -shared -o lib739kv.so $(CXXLINK)

clean:
	rm -f core *.o output

