DBGFLAGS=-g -fsanitize=address -fsanitize=undefined
#DBGFLAGS=-g -fsanitize=thread

override LDFLAGS :=$(LDFLAGS) -pthread

all:
	g++ main.cpp $(LDFLAGS) $(DBGFLAGS)
	./a.out