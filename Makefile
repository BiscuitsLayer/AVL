CC = g++
LDFLAGS = -std=c++2a
CXXFLAGS = -MMD -O3 -ggdb3 -std=c++2a
Test = 3

all: main run

main: main.o

check: test
	./test

test: test.o -lgtest -lpthread

image:
	dot -Tpng $(CURDIR)/Images/Tree.dot -o $(CURDIR)/Images/Tree.png

run:
	./main 

.PHONY = all clean

-include *.d