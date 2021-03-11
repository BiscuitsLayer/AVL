

all:	b r
b:
		g++ test.cpp -ggdb3 -o test -lgtest -lpthread
r:
		./test
image:
		dot -Tpng $(CURDIR)/Images/Tree.dot -o $(CURDIR)/Images/Tree.png
