//	SYSTEM
#include <fstream>
#include <set>
#include <chrono>

//	TREE
#include "Tree/Tree.hpp"

using StdTree = std::set <int>;
using StdIterator = std::set <int>::iterator;

using MyTree = AVL::Tree <int>;
using MyIterator = AVL::Tree <int>::Iterator;

void MakeDot (const AVL::Tree <int>& tree) {
	std::ofstream treeDot  { "Images/Tree.dot", std::ios::trunc };
	if (!treeDot.is_open ()) {
		std::cerr << "Error opening dot file!" << std::endl;
	}
	tree.MakeDot (&treeDot);
}

int main (int argc, char** argv) {
	StdTree stdSet_ {};
    MyTree mySet_ {};

	int elementsCount = 0;
	std::cin >> elementsCount;
    std::vector <int> vec {};
    vec.reserve (elementsCount);
    int temp = 0;
    for (int i = 0; i < elementsCount; ++i) {
        std::cin >> temp;
        vec.push_back (temp);
	}
    
    stdSet_ = { vec.begin (), vec.end () };
    mySet_ = { vec };
	
    //  PREPARING REQUESTS
    int requestsCount = 0;
    std::cin >> requestsCount;
    std::vector <std::pair <int, int>> requests {};
    int first = 0, second = 0;
    for (int i = 0; i < requestsCount; ++i) {
        std::cin >> first >> second;
        requests.emplace_back (first, second);
	}
        
    //  PREPARING VECTORS FOR ANSWERS
    std::vector <int> stdAns {};
    stdAns.reserve (requestsCount);
    std::vector <int> myAns {};
    myAns.reserve (requestsCount);

    //  TESTING STD SET
    auto stdStart = std::chrono::steady_clock::now ();
    for (auto&& request : requests) {
        auto ans = std::distance (stdSet_.lower_bound (request.first), stdSet_.upper_bound (request.second));
        stdAns.push_back (ans);
    }
    auto stdEnd = std::chrono::steady_clock::now ();
        
    //  TESTING AVL TREE
    auto myStart = std::chrono::steady_clock::now ();
    for (auto&& request : requests) {
        auto ans = std::distance (mySet_.LowerBound (request.first), mySet_.UpperBound (request.second));
        myAns.push_back (ans);
    }
    auto myEnd = std::chrono::steady_clock::now ();

    //  CHECK RESULTS
    for (int i = 0; i < requestsCount; ++i) {
		if (stdAns[i] != myAns[i]) {
			std::cerr << "Error! Answers don't match!" << std::endl;
			return 0;
		}
		std::cout << (i == 0 ? "" : " ") << myAns[i];
	}
	std::cout << std::endl;
    
    std::cerr << "std::set time  = " << (stdEnd - stdStart).count () << std::endl;
    std::cerr << "AVL::Tree time = " << (myEnd - myStart).count () << std::endl;

	MakeDot (mySet_);
	return 0;
}
