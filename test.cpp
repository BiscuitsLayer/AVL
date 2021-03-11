//  SYSTEM
#include <fstream>

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

		void DoTest (int elementsCount) {
			std::vector <int> vec {};
            vec.reserve (elementsCount);
            for (int i = 0; i < elementsCount; ++i) {
                vec.push_back (rand () % elementsCount);
            }
            for (auto&& i : vec) {
                stdSet_.insert (i);
                mySet_.Insert (i);
            }

            auto stdIt = stdSet_.begin ();
            auto myIt = mySet_.begin ();
            for (; (stdIt != stdSet_.end ()) || (myIt != mySet_.end ()); ++stdIt, ++myIt) {
                TestFunc (stdIt, myIt);
            }
		}

        virtual void TestFunc (StdIterator stdIt, MyIterator myIt) = 0;
};

class Equal : public AVLTest { 
    virtual void TestFunc (StdIterator stdIt, MyIterator myIt) override {
        ASSERT_EQ (*stdIt, *myIt);
    } 
};

class UpperBound : public AVLTest { 
    virtual void TestFunc (StdIterator stdIt, MyIterator myIt) override {
        ASSERT_EQ (*stdIt, *myIt);
        auto stdUpper = std::upper_bound (mySet_.begin (), mySet_.end (), *myIt);
        auto myUpper = mySet_.UpperBound (*myIt);
        ASSERT_EQ (stdUpper, myUpper);
    } 
};

class LowerBound : public AVLTest { 
    virtual void TestFunc (StdIterator stdIt, MyIterator myIt) override {
        ASSERT_EQ (*stdIt, *myIt);
        auto stdUpper = std::lower_bound (mySet_.begin (), mySet_.end (), *myIt);
        auto myUpper = mySet_.LowerBound (*myIt);
        ASSERT_EQ (stdUpper, myUpper);
    } 
};

TEST_F (Equal, Test) { DoTest (1000); }
TEST_F (UpperBound, Test) { DoTest (1000); }
TEST_F (LowerBound, Test) { DoTest (1000); }


int main (int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}