#include <fstream>

#include "Tree/Tree.hpp"
#include <set>

void MakeDot (AVL::Tree tree) {
	std::ofstream treeDot  { "Images/Tree.dot", std::ios::trunc };
	if (!treeDot.is_open ()) {
		std::cerr << "Error opening dot file!" << std::endl;
	}
	tree.MakeDot (&treeDot);
}

int main () {
	AVL::Tree tree {};

	tree.Insert ('b');
	tree.Insert ('a');
	tree.Insert ('q');
	tree.Insert ('d');
	tree.Insert ('c');

	std::ofstream treeDot  { "Images/Tree.dot", std::ios::trunc };
	if (!treeDot.is_open ()) {
		std::cerr << "Error opening dot file!" << std::endl;
	}
	tree.MakeDot (&treeDot);
	//MakeDot (temp);
	return 0;
}
