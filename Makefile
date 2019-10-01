# Makefile
# Requires that the object definitions be in wl.h and wl.cpp

# CXX = g++
# CXXFLAGS = -O2 -g -Wall -std=c++11
CXX = gcc
CXXLINK = -lcpprest -lboost_system -lcrypto -lssl

# During debugging you may want to used the compiler flags listed below
CXXFLAGS = -O2 -g -Wall -std=c++11

all:
	make lib739kv.so
	make performance_test
	make correctness_test

performance_test: performance_test.c
	$(CXX) performance_test.c -L. -l :lib739kv.so -o performance_test

correctness_test: correctness_test.c
	$(CXX) correctness_test.c -L. -l :lib739kv.so -o correctness_test

lib739kv.so: 739kv.h 739kv.cpp
	rm -f *.so
	g++ $(CXXFLAGS) 739kv.cpp -fPIC -shared -o lib739kv.so $(CXXLINK)

clean:
	rm -f core *.o output
