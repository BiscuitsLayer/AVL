//	SYSTEM
#include <fstream>

//	TREE
#include "Tree/Tree.hpp"

void MakeDot (const AVL::Tree <int>& tree) {
	std::ofstream treeDot  { "Images/Tree.dot", std::ios::trunc };
	if (!treeDot.is_open ()) {
		std::cerr << "Error opening dot file!" << std::endl;
	}
	tree.MakeDot (&treeDot);
}

int main (int argc, char** argv) {
	AVL::Tree <int> tree {};

	MakeDot (tree);
	return 0;
}
