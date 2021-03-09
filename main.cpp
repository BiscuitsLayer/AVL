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

AVL::Tree AddedX (AVL::Tree tree) {
	tree.Insert ('x');
	return tree;
}

void CtorsTest () {
	AVL::Tree tree1 {};
	tree1.Insert ('x');
	tree1.Insert ('z');
	tree1.Insert ('y');
	AVL::Tree tree2 {};
	tree2 = tree1;
	tree2 = AddedX (tree1);
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
	
	CtorsTest ();
	return 0;
}
