//	SYSTEM
#include <iostream>
#include <fstream>
#include <set>
#include <chrono>

//	TREE
#include "Source/Tree.hpp"

using StdTree = std::set <int>;
using StdIterator = std::set <int>::iterator;

using MyTree = AVL::Tree <int>;
using MyIterator = AVL::Tree <int>::Iterator;

void MakeDot (const AVL::Tree <int>& tree) {
	std::ofstream treeDot  { "Images/Tree.dot", std::ios::trunc };
	if (!treeDot.is_open ()) {
		std::cerr << "Error opening dot file!" << std::endl;
	}
	tree.dump (&treeDot);
}

int main (int argc, char** argv) {
    /*
    std::ifstream inputStream { argv[1] };
	if (!inputStream) {
		std::cerr << "Error opening file!" << std::endl;
		return 0;
	}

	StdTree stdSet_ {};
    MyTree mySet_ {};

	int elementsCount = 0;
	inputStream >> elementsCount;
    std::vector <int> vec {};
    vec.reserve (elementsCount);
    int temp = 0;
    for (int i = 0; i < elementsCount; ++i) {
        inputStream >> temp;
        vec.push_back (temp);
	}
    
    stdSet_ = { vec.begin (), vec.end () };
    mySet_ = { vec };
	
    //  PREPARING REQUESTS
    int requestsCount = 0;
    inputStream >> requestsCount;
    std::vector <std::pair <int, int>> requests {};
    int first = 0, second = 0;
    for (int i = 0; i < requestsCount; ++i) {
        inputStream >> first >> second;
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
        auto ans = std::distance (mySet_.lower_bound (request.first), mySet_.upper_bound (request.second));
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
    */
    std::vector <int> vec { 0, 0, 0, 9, 9, 999, 1000, 500, 100, 750, 250, 6 };
    AVL::Tree <int> tree1 { vec };
    
    for (auto it = tree1.begin (),
    it2 = tree1.end ();
    it != it2;
    ++it) {
        std::cout << *it << '\n';
        if (*it % 2 == 0) {
            --it;
            std::cout << *it << '\n';
            ++it;
        }
    }
    
    MakeDot (tree1);
	return 0;
}
