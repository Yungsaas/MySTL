/*
 * THESE TESTS WERE WRITTEN WITH THE HELP OF AI
 */

#include "../extras/catch_amalgamated.hpp"
#include "../source/forward_list.hpp"
#include <string>

using namespace mystl;

TEST_CASE("forward_list: Construction and basic queries", "[forward_list]") {
    SECTION("Default constructor creates empty list") {
        my_forward_list<int> list;
        REQUIRE(list.empty());
        REQUIRE(list.size() == 0);
    }

    SECTION("Push front increases size") {
        my_forward_list<int> list;
        list.push_front(42);
        REQUIRE_FALSE(list.empty());
        REQUIRE(list.size() == 1);
        REQUIRE(list.front() == 42);
    }
}

TEST_CASE("forward_list: Push and pop operations", "[forward_list]") {
    SECTION("Push front multiple elements") {
        my_forward_list<int> list;
        list.push_front(1);
        list.push_front(2);
        list.push_front(3);
        
        REQUIRE(list.size() == 3);
        REQUIRE(list.front() == 3);
    }

    SECTION("Pop front removes elements correctly") {
        my_forward_list<int> list;
        list.push_front(1);
        list.push_front(2);
        list.push_front(3);
        
        list.pop_front();
        REQUIRE(list.size() == 2);
        REQUIRE(list.front() == 2);
        
        list.pop_front();
        REQUIRE(list.size() == 1);
        REQUIRE(list.front() == 1);
        
        list.pop_front();
        REQUIRE(list.empty());
    }

    SECTION("Pop front on empty list throws exception") {
        my_forward_list<int> list;
        REQUIRE_THROWS_AS(list.pop_front(), std::out_of_range);
    }

    SECTION("Push front with move semantics") {
        my_forward_list<std::string> list;
        std::string s = "hello";
        list.push_front(std::move(s));
        
        REQUIRE(list.front() == "hello");
        // s should be moved from (in valid but unspecified state)
    }
}

TEST_CASE("forward_list: Front access", "[forward_list]") {
    SECTION("Front returns reference to first element") {
        my_forward_list<int> list;
        list.push_front(10);
        list.push_front(20);
        
        REQUIRE(list.front() == 20);
        
        // Modify through reference
        list.front() = 99;
        REQUIRE(list.front() == 99);
    }

    SECTION("Front on empty list throws exception") {
        my_forward_list<int> list;
        REQUIRE_THROWS_AS(list.front(), std::out_of_range);
    }

    SECTION("Const front access") {
        my_forward_list<int> list;
        list.push_front(42);
        
        const my_forward_list<int>& const_list = list;
        REQUIRE(const_list.front() == 42);
    }
}

TEST_CASE("forward_list: Clear operation", "[forward_list]") {
    SECTION("Clear removes all elements") {
        my_forward_list<int> list;
        list.push_front(1);
        list.push_front(2);
        list.push_front(3);
        
        list.clear();
        REQUIRE(list.empty());
        REQUIRE(list.size() == 0);
    }

    SECTION("Clear on empty list is safe") {
        my_forward_list<int> list;
        list.clear();
        REQUIRE(list.empty());
    }

    SECTION("Can push after clear") {
        my_forward_list<int> list;
        list.push_front(1);
        list.clear();
        list.push_front(2);
        
        REQUIRE(list.size() == 1);
        REQUIRE(list.front() == 2);
    }
}

TEST_CASE("forward_list: Copy constructor", "[forward_list]") {
    SECTION("Copy constructor creates independent copy") {
        my_forward_list<int> list1;
        list1.push_front(1);
        list1.push_front(2);
        list1.push_front(3);
        
        my_forward_list<int> list2(list1);
        
        REQUIRE(list2.size() == 3);
        REQUIRE(list2.front() == 3);
        REQUIRE(list1 == list2);
        
        // Modify list2, list1 should be unchanged
        list2.pop_front();
        REQUIRE(list1.size() == 3);
        REQUIRE(list2.size() == 2);
    }

    SECTION("Copy empty list") {
        my_forward_list<int> list1;
        my_forward_list<int> list2(list1);
        
        REQUIRE(list2.empty());
        REQUIRE(list1 == list2);
    }
}

TEST_CASE("forward_list: Copy assignment", "[forward_list]") {
    SECTION("Copy assignment works correctly") {
        my_forward_list<int> list1;
        list1.push_front(1);
        list1.push_front(2);
        
        my_forward_list<int> list2;
        list2.push_front(99);
        
        list2 = list1;
        
        REQUIRE(list2.size() == 2);
        REQUIRE(list2.front() == 2);
        REQUIRE(list1 == list2);
    }

    SECTION("Self assignment is safe") {
        my_forward_list<int> list;
        list.push_front(1);
        list.push_front(2);
        
        list = list;
        
        REQUIRE(list.size() == 2);
        REQUIRE(list.front() == 2);
    }

    SECTION("Assign to non-empty list") {
        my_forward_list<int> list1;
        list1.push_front(1);
        
        my_forward_list<int> list2;
        list2.push_front(99);
        list2.push_front(98);
        list2.push_front(97);
        
        list2 = list1;
        
        REQUIRE(list2.size() == 1);
        REQUIRE(list2.front() == 1);
    }
}

TEST_CASE("forward_list: Move constructor", "[forward_list]") {
    SECTION("Move constructor transfers ownership") {
        my_forward_list<int> list1;
        list1.push_front(1);
        list1.push_front(2);
        list1.push_front(3);
        
        my_forward_list<int> list2(std::move(list1));
        
        REQUIRE(list2.size() == 3);
        REQUIRE(list2.front() == 3);
        REQUIRE(list1.empty());
        REQUIRE(list1.size() == 0);
    }
}

TEST_CASE("forward_list: Move assignment", "[forward_list]") {
    SECTION("Move assignment transfers ownership") {
        my_forward_list<int> list1;
        list1.push_front(1);
        list1.push_front(2);
        
        my_forward_list<int> list2;
        list2.push_front(99);
        
        list2 = std::move(list1);
        
        REQUIRE(list2.size() == 2);
        REQUIRE(list2.front() == 2);
        REQUIRE(list1.empty());
    }

    SECTION("Self move assignment is safe") {
        my_forward_list<int> list;
        list.push_front(1);
        list.push_front(2);
        
        list = std::move(list);
        
        // After self-move, list is in valid state
        REQUIRE(list.size() >= 0);
    }
}

TEST_CASE("forward_list: Iterators", "[forward_list]") {
    SECTION("Begin and end iterators") {
        my_forward_list<int> list;
        list.push_front(3);
        list.push_front(2);
        list.push_front(1);
        
        auto it = list.begin();
        REQUIRE(*it == 1);
        ++it;
        REQUIRE(*it == 2);
        ++it;
        REQUIRE(*it == 3);
        ++it;
        REQUIRE(it == list.end());
    }

    SECTION("Empty list iterators") {
        my_forward_list<int> list;
        REQUIRE(list.begin() == list.end());
    }

    SECTION("Iterator traversal") {
        my_forward_list<int> list;
        list.push_front(3);
        list.push_front(2);
        list.push_front(1);
        
        int sum = 0;
        for (auto it = list.begin(); it != list.end(); ++it) {
            sum += *it;
        }
        REQUIRE(sum == 6);
    }

    SECTION("Range-based for loop") {
        my_forward_list<int> list;
        list.push_front(3);
        list.push_front(2);
        list.push_front(1);
        
        int sum = 0;
        for (int val : list) {
            sum += val;
        }
        REQUIRE(sum == 6);
    }

    SECTION("Post-increment iterator") {
        my_forward_list<int> list;
        list.push_front(2);
        list.push_front(1);
        
        auto it = list.begin();
        auto old_it = it++;
        
        REQUIRE(*old_it == 1);
        REQUIRE(*it == 2);
    }

    SECTION("Const iterators") {
        my_forward_list<int> list;
        list.push_front(3);
        list.push_front(2);
        list.push_front(1);
        
        const my_forward_list<int>& const_list = list;
        
        int sum = 0;
        for (auto it = const_list.begin(); it != const_list.end(); ++it) {
            sum += *it;
        }
        REQUIRE(sum == 6);
    }

    SECTION("Cbegin and cend") {
        my_forward_list<int> list;
        list.push_front(2);
        list.push_front(1);
        
        auto it = list.cbegin();
        REQUIRE(*it == 1);
        // it is const_iterator, so can't modify through it
    }
}

TEST_CASE("forward_list: Before begin iterator", "[forward_list]") {
    SECTION("Before begin points to sentinel") {
        my_forward_list<int> list;
        list.push_front(1);
        
        auto bb = list.before_begin();
        ++bb;
        REQUIRE(*bb == 1);
    }

    SECTION("Cbefore_begin") {
        my_forward_list<int> list;
        list.push_front(1);
        
        auto bb = list.cbefore_begin();
        ++bb;
        REQUIRE(*bb == 1);
    }
}

TEST_CASE("forward_list: Insert after", "[forward_list]") {
    SECTION("Insert after begin") {
        my_forward_list<int> list;
        list.push_front(1);
        
        auto it = list.begin();
        list.insert_after(it, 2);
        
        REQUIRE(list.size() == 2);
        REQUIRE(list.front() == 1);
        
        ++it;
        REQUIRE(*it == 2);
    }

    SECTION("Insert after before_begin") {
        my_forward_list<int> list;
        list.push_front(2);
        
        list.insert_after(list.before_begin(), 1);
        
        REQUIRE(list.size() == 2);
        REQUIRE(list.front() == 1);
    }

    SECTION("Insert after returns iterator to new element") {
        my_forward_list<int> list;
        list.push_front(1);
        
        auto new_it = list.insert_after(list.begin(), 2);
        REQUIRE(*new_it == 2);
    }

    SECTION("Insert after with move semantics") {
        my_forward_list<std::string> list;
        list.push_front("first");
        
        std::string s = "second";
        list.insert_after(list.begin(), std::move(s));
        
        auto it = list.begin();
        ++it;
        REQUIRE(*it == "second");
    }

    SECTION("Multiple insert after operations") {
        my_forward_list<int> list;
        list.push_front(1);
        
        auto it = list.begin();
        it = list.insert_after(it, 2);
        it = list.insert_after(it, 3);
        list.insert_after(it, 4);
        
        REQUIRE(list.size() == 4);
        
        // List should be: 1, 2, 3, 4
        auto check_it = list.begin();
        REQUIRE(*check_it == 1);
        ++check_it;
        REQUIRE(*check_it == 2);
        ++check_it;
        REQUIRE(*check_it == 3);
        ++check_it;
        REQUIRE(*check_it == 4);
    }
}

TEST_CASE("forward_list: Erase after", "[forward_list]") {
    SECTION("Erase after begin") {
        my_forward_list<int> list;
        list.push_front(3);
        list.push_front(2);
        list.push_front(1);
        
        auto it = list.begin();
        auto next_it = list.erase_after(it);
        
        REQUIRE(list.size() == 2);
        REQUIRE(*next_it == 3);
        REQUIRE(list.front() == 1);
    }

    SECTION("Erase after before_begin") {
        my_forward_list<int> list;
        list.push_front(2);
        list.push_front(1);
        
        list.erase_after(list.before_begin());
        
        REQUIRE(list.size() == 1);
        REQUIRE(list.front() == 2);
    }

    SECTION("Erase after at end throws exception") {
        my_forward_list<int> list;
        list.push_front(1);
        
        auto it = list.begin();
        REQUIRE_THROWS_AS(list.erase_after(it), std::out_of_range);
    }

    SECTION("Erase all elements using erase_after") {
        my_forward_list<int> list;
        list.push_front(3);
        list.push_front(2);
        list.push_front(1);
        
        while (!list.empty()) {
            list.erase_after(list.before_begin());
        }
        
        REQUIRE(list.empty());
    }

    SECTION("Erase after returns iterator to next element") {
        my_forward_list<int> list;
        list.push_front(3);
        list.push_front(2);
        list.push_front(1);
        
        auto it = list.begin();
        ++it; // Point to 2
        auto next = list.erase_after(it); // Erase 3
        
        REQUIRE(next == list.end());
    }
}

TEST_CASE("forward_list: Equality operators", "[forward_list]") {
    SECTION("Equal lists compare equal") {
        my_forward_list<int> list1;
        list1.push_front(3);
        list1.push_front(2);
        list1.push_front(1);
        
        my_forward_list<int> list2;
        list2.push_front(3);
        list2.push_front(2);
        list2.push_front(1);
        
        REQUIRE(list1 == list2);
        REQUIRE_FALSE(list1 != list2);
    }

    SECTION("Different size lists are not equal") {
        my_forward_list<int> list1;
        list1.push_front(1);
        
        my_forward_list<int> list2;
        list2.push_front(1);
        list2.push_front(2);
        
        REQUIRE_FALSE(list1 == list2);
        REQUIRE(list1 != list2);
    }

    SECTION("Different element lists are not equal") {
        my_forward_list<int> list1;
        list1.push_front(1);
        list1.push_front(2);
        
        my_forward_list<int> list2;
        list2.push_front(1);
        list2.push_front(3);
        
        REQUIRE_FALSE(list1 == list2);
        REQUIRE(list1 != list2);
    }

    SECTION("Empty lists are equal") {
        my_forward_list<int> list1;
        my_forward_list<int> list2;
        
        REQUIRE(list1 == list2);
    }
}

TEST_CASE("forward_list: Complex types", "[forward_list]") {
    SECTION("Forward list of strings") {
        my_forward_list<std::string> list;
        list.push_front("world");
        list.push_front("hello");
        
        REQUIRE(list.front() == "hello");
        REQUIRE(list.size() == 2);
        
        list.pop_front();
        REQUIRE(list.front() == "world");
    }

    SECTION("Forward list of pairs") {
        my_forward_list<std::pair<int, std::string>> list;
        list.push_front({2, "two"});
        list.push_front({1, "one"});
        
        REQUIRE(list.front().first == 1);
        REQUIRE(list.front().second == "one");
    }
}

TEST_CASE("forward_list: Stress test", "[forward_list]") {
    SECTION("Large number of elements") {
        my_forward_list<int> list;
        const int N = 1000;
        
        for (int i = 0; i < N; ++i) {
            list.push_front(i);
        }
        
        REQUIRE(list.size() == N);
        
        int count = 0;
        for (int val : list) {
            (void)val;
            count++;
        }
        REQUIRE(count == N);
        
        list.clear();
        REQUIRE(list.empty());
    }

    SECTION("Alternating push and pop") {
        my_forward_list<int> list;
        
        for (int i = 0; i < 100; ++i) {
            list.push_front(i);
            list.push_front(i + 1);
            list.pop_front();
        }
        
        REQUIRE(list.size() == 100);
    }
}