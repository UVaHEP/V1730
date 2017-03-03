INCDIR := include/


LIBS := -lcaen
CAENLIBS := -lCAENComm
CXXFLAGS := --std=c++11 -fPIC -Wall -g
LDFLAGS := -Llib 
CPPFLAGS:= -I. -I$(INCDIR) 
CWD := $(shell pwd)
RPATH := -Wl,-rpath=$(CWD)/lib




all: bin/readDataBlocks bin/commTest


bin/readDataBlocks: src/readDataBlocks.cxx lib/libcaen.so
	g++ $(CXXFLAGS) $(CPPFLAGS) -o bin/readDataBlocks src/readDataBlocks.cxx $(LIBS) $(CAENLIBS) $(LDFLAGS) $(RPATH)

bin/commTest: src/commTest.cxx lib/libcaen.so
	g++ $(CXXFLAGS) $(CPPFLAGS) -o bin/commTest src/commTest.cxx $(LIBS) $(CAENLIBS) $(LDFLAGS) $(RPATH)

lib/libcaen.so: src/caen.cxx
	g++ $(CXXFLAGS) $(CPPFLAGS) -shared -o lib/libcaen.so src/caen.cxx $(CAENLIBS)

clean:
	rm lib/* bin/*
