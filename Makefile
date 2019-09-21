# Makefile
# Requires that the object definitions be in wl.h and wl.cpp

CXX = g++
CXXFLAGS = -O2 -g -Wall -std=c++11 
CXXLINK = -lcpprest -lboost_system -lcrypto -lssl 
OBJECTS = main.cpp

# During debugging you may want to used the compiler flags listed below
#CXXFLAGS =      -g -Wall

all: main.cpp
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o output $(CXXLINK)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -L. -l739kv -o output

lib739kv.so: lib739kv.h 
	$(CXX) $(CXXFLAGS) lib739kv.h -fPIC -shared -o lib739kv.so $(CXXLINK)

clean:
	rm -f core *.o output

