#include <fstream>
#include <unistd.h>

#include "Tree/Tree.hpp"
#include <set>

void MakeDot (AVL::Tree <char> tree) {
	std::ofstream treeDot  { "Images/Tree.dot", std::ios::trunc };
	if (!treeDot.is_open ()) {
		std::cerr << "Error opening dot file!" << std::endl;
	}
	tree.MakeDot (&treeDot);
}

AVL::Tree <char> AddedX (AVL::Tree <char> tree) {
	tree.Insert ('x');
	return tree;
}

void CtorsTest () {
	AVL::Tree <char> tree1 {};
	tree1.Insert ('x');
	tree1.Insert ('z');
	tree1.Insert ('y');
	AVL::Tree <char> tree2 {};
	tree2 = tree1;
	tree2 = AddedX (tree1);
	MakeDot (tree1);
}

int main () {
	AVL::Tree <char> tree {};

	tree.Insert ('b');
	tree.Insert ('a');
	tree.Insert ('q');
	tree.Insert ('d');
	tree.Insert ('c');
	tree.Insert ('x');


	std::ofstream treeDot  { "Images/Tree.dot", std::ios::trunc };
	if (!treeDot.is_open ()) {
		std::cerr << "Error opening dot file!" << std::endl;
	}
	tree.MakeDot (&treeDot);
	CtorsTest ();
	std::cout << *tree.LowerBound ('a') << std::endl;
	std::cout << *tree.UpperBound ('a') << std::endl;
}
