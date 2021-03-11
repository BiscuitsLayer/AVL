//  SYSTEM
#include <fstream>
#include <chrono>

//  TREE
#include "Tree/Tree.hpp"

//	GOOGLE TEST
#include <gtest/gtest.h>

using StdTree = std::set <int>;
using StdIterator = std::set <int>::iterator;

using MyTree = AVL::Tree <int>;
using MyIterator = AVL::Tree <int>::Iterator;

class AVLTest : public ::testing::Test {
	public:
        MyTree mySet_ {};
        StdTree stdSet_ {};

        //  RANDOM FILL
        void RandomFill (int elementsCount) {
            std::vector <int> vec {};
            vec.reserve (elementsCount);
            for (int i = 0; i < elementsCount; ++i) {
                vec.push_back (rand () % elementsCount);
            }

            stdSet_ = { vec.begin (), vec.end () };
            mySet_ = { vec };
        }

        //  COMMON FUNCTION TEST
		void DoTest (int elementsCount) {
			RandomFill (elementsCount);

            auto stdIt = stdSet_.begin ();
            auto myIt = mySet_.begin ();

            for (; (stdIt != stdSet_.end ()) || (myIt != mySet_.end ()); ++stdIt, ++myIt) {
                TestFunc (stdIt, myIt);
            }
		}

        //  FUNCTION TO TEST
        virtual void TestFunc (StdIterator stdIt, MyIterator myIt) { /* empty */ }

        //  SPEED TEST
        void SpeedTest (int elementsCount) {
            //  FILLING THE TREES
            #ifndef NO_RANDOM_FILL
                RandomFill (elementsCount);

                int requestsCount = rand () % elementsCount;
                std::vector <std::pair <int, int>> requests {};

                for (int i = 0; i < requestsCount; ++i) {
                    requests.emplace_back (rand () % elementsCount, rand () % elementsCount);
                }
            #else
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
                auto ans = std::distance (mySet_.LowerBound (request.first), mySet_.UpperBound (request.second));
                myAns.push_back (ans);
            }
            auto myEnd = std::chrono::steady_clock::now ();

            //  CHECK RESULTS
            ASSERT_EQ (stdAns, myAns);
            
            std::cerr << "std::set time  = " << (stdEnd - stdStart).count () << std::endl;
            std::cerr << "AVL::Tree time = " << (myEnd - myStart).count () << std::endl;
        }
};

class Equal : public AVLTest { 
    void TestFunc (StdIterator stdIt, MyIterator myIt) override {
        ASSERT_EQ (*stdIt, *myIt);
    } 
};

class UpperBound : public AVLTest { 
    void TestFunc (StdIterator stdIt, MyIterator myIt) override {
        ASSERT_EQ (*stdIt, *myIt);
        auto stdUpper = std::upper_bound (mySet_.begin (), mySet_.end (), *myIt);
        auto myUpper = mySet_.UpperBound (*myIt);
        ASSERT_EQ (stdUpper, myUpper);
    } 
};

class LowerBound : public AVLTest { 
    void TestFunc (StdIterator stdIt, MyIterator myIt) override {
        ASSERT_EQ (*stdIt, *myIt);
        auto stdUpper = std::lower_bound (mySet_.begin (), mySet_.end (), *myIt);
        auto myUpper = mySet_.LowerBound (*myIt);
        ASSERT_EQ (stdUpper, myUpper);
    } 
};

TEST_F (Equal, Test) { DoTest (1000); }
TEST_F (UpperBound, Test) { DoTest (1000); }
TEST_F (LowerBound, Test) { DoTest (1000); }
TEST_F (AVLTest, SpeedTest) { SpeedTest (1000); }

int main (int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}