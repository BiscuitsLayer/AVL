

all:	b r image
b:
		g++ main.cpp -ggdb3 -o main
r:
		./main
image:
		dot -Tpng $(CURDIR)/Images/Tree.dot -o $(CURDIR)/Images/Tree.png
