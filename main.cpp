//	SYSTEM
#include <iostream>
#include <fstream>
#include <set>
#include <chrono>

//	TREE
#include "Source/Tree.hpp"

using StdTree = std::set <int>;
using MyTree = AVL::Tree <int>;

#define MAIN_PROGRAM
//#define RANDOM_FILL

void MakeDot (const AVL::Tree <int>& tree) {
	std::ofstream treeDot  { "Images/Tree.dot", std::ios::trunc };
	if (!treeDot.is_open ()) {
		std::cerr << "Error opening dot file!" << std::endl;
	}
	tree.dump (&treeDot);
}

int main (int argc, char** argv) {
#ifdef MAIN_PROGRAM

    //  VECTOR FILL
#ifdef RANDOM_FILL
    int elementsCount = 30;
    StdTree stdSet_ {};
    MyTree mySet_ {};

    std::vector <int> vec {};
    vec.reserve (elementsCount);
    for (int i = 0; i < elementsCount; ++i) {
        vec.push_back (rand () % elementsCount);
    }
#else
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
#endif

    stdSet_ = { vec.begin (), vec.end () };
    mySet_ = { vec };
	
    //  PREPARING REQUESTS
#ifdef RANDOM_FILL
    int requestsCount = rand () % elementsCount;
    std::vector <std::pair <int, int>> requests {};
    for (int i = 0; i < requestsCount; ++i) {
        auto pair = std::make_pair (rand () % elementsCount, rand () % elementsCount);
        if (pair.first > pair.second) {
            std::swap (pair.first, pair.second);
        }
        requests.push_back (pair);
    }
#else
    int requestsCount = 0;
    inputStream >> requestsCount;
    std::vector <std::pair <int, int>> requests {};
    int first = 0, second = 0;
    for (int i = 0; i < requestsCount; ++i) {
        inputStream >> first >> second;
        requests.emplace_back (first, second);
	}
#endif
    
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
#else
    std::vector <int> testVec = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
    AVL::Tree <int> testTree { testVec };
    for (auto&& elem : testTree) {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;
#endif
	return 0;
}
