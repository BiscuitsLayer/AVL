#include <fstream>

#include "Tree/Tree.hpp"

int main () {
	AVL::Tree temp {};

	temp.Insert ('b');
	temp.Insert ('a');
	temp.Insert ('q');
	temp.Insert ('d');
	temp.Insert ('c');

	std::ofstream treeDot  { "Images/Tree.dot", std::ios::trunc };
	if (!treeDot.is_open ()) {
		std::cerr << "Error opening dot file!" << std::endl;
	}
	temp.MakeDot (&treeDot);
	return 0;
}
