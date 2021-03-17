//	SYSTEM
#include <iostream>
#include <fstream>
#include <set>
#include <chrono>

//	TREE
#include "Source/Tree.hpp"

using StdTree = std::set <int>;
using MyTree = AVL::Tree <int>;

//#define MAIN_PROGRAM

void MakeDot (const AVL::Tree <int>& tree) {
	std::ofstream treeDot  { "Images/Tree.dot", std::ios::trunc };
	if (!treeDot.is_open ()) {
		std::cerr << "Error opening dot file!" << std::endl;
	}
	tree.dump (&treeDot);
}

int main (int argc, char** argv) {
#ifdef MAIN_PROGRAM
    int elementsCount = 10;
    StdTree stdSet_ {};
    MyTree mySet_ {};

    std::vector <int> vec {};
    vec.reserve (elementsCount);
    for (int i = 0; i < elementsCount; ++i) {
        vec.push_back (rand () % elementsCount);
    }
    stdSet_ = { vec.begin (), vec.end () };
    mySet_ = { vec };
    /*
    //std::ifstream inputStream { argv[1] };
    std::ifstream inputStream { "Test/3.in" };
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
    */
    int requestsCount = rand () % elementsCount;
    std::vector <std::pair <int, int>> requests {};
    for (int i = 0; i < requestsCount; ++i) {
        auto pair = std::make_pair (rand () % elementsCount, rand () % elementsCount);
        if (pair.first > pair.second) {
            std::swap (pair.first, pair.second);
        }
        requests.push_back (pair);
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
    std::cout << "--------------- SET DEBUG ---------------" << std::endl;
    for (auto&& i : mySet_) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
    std::cout << "--------------- SET DEBUG ---------------" << std::endl;

    std::cout << "Measure start" << std::endl;
    auto myStart = std::chrono::steady_clock::now ();
    for (auto&& request : requests) {
        auto lower = mySet_.lower_bound (request.first);
        std::cout << "lower done: " << *lower << std::endl;
        auto upper = mySet_.upper_bound (request.second);
        std::cout << "upper done: " << *upper << std::endl;
        auto ans = std::distance (lower, upper);
        //auto ans = std::distance (mySet_.lower_bound (request.first), mySet_.upper_bound (request.second));
        myAns.push_back (ans);
    }
    auto myEnd = std::chrono::steady_clock::now ();
    std::cout << "Measure finish" << std::endl;

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
#endif
    
    std::vector <int> testVec = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
    AVL::Tree <int> testTree { testVec };
    for (auto&& elem : testTree) {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;
	return 0;
}
