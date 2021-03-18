CC = g++
LDFLAGS = -std=c++2a
CXXFLAGS = -MMD -O0 -ggdb3 -std=c++2a
Test = 4

all: main run 

main: main.o

check: test
	./test

test: test.o -lgtest -lpthread

image:
	dot -Tpng $(CURDIR)/Images/Tree.dot -o $(CURDIR)/Images/Tree.png

run:
	./main Test/$(Test).in

clean:
	rm -r *.d *.o main test

.PHONY = all clean

-include *.d