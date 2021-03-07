

all:	b r image
b:
		g++ main.cpp -ggdb3 -o main
r:
		./main $(CURDIR)
image:
		system ("dot -Tpng /home/biscuitslayer/CLionProjects/8_Differentiator/temp.dot -o/home/biscuitslayer/CLionProjects/8_Differentiator/temp.png");
