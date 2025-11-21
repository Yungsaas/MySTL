/*
 * THESE TESTS WERE WRITTEN WITH THE HELP OF AI
 */
#include "../source/vector.hpp"
#include "../extras/catch_amalgamated.hpp"

TEST_CASE("vector default constructor") {
    mystl::my_vector<int> v;
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);
    REQUIRE(v.empty());
}

TEST_CASE("vector push_back increases size") {
    mystl::my_vector<int> v;
    v.push_back(10);
    v.push_back(20);
    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == 10);
    REQUIRE(v[1] == 20);
}

TEST_CASE("vector reserve increases capacity but not size") {
    mystl::my_vector<int> v;
    v.reserve(50);

    REQUIRE(v.capacity() >= 50);
    REQUIRE(v.size() == 0);
}

TEST_CASE("vector pop_back removes last element") {
    mystl::my_vector<int> v;
    v.push_back(5);
    v.push_back(9);
    v.pop_back();
    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 5);
}

TEST_CASE("vector copy constructor") {
    mystl::my_vector<int> a;
    a.push_back(1);
    a.push_back(2);

    mystl::my_vector<int> b = a;

    REQUIRE(b.size() == 2);
    REQUIRE(b[0] == 1);
    REQUIRE(b[1] == 2);
}

TEST_CASE("vector copy assignment") {
    mystl::my_vector<int> a;
    a.push_back(3);
    a.push_back(7);

    mystl::my_vector<int> b;
    b = a;

    REQUIRE(b.size() == 2);
    REQUIRE(b[0] == 3);
    REQUIRE(b[1] == 7);
}

TEST_CASE("vector move constructor") {
    mystl::my_vector<int> a;
    a.push_back(42);

    mystl::my_vector<int> b = std::move(a);

    REQUIRE(b.size() == 1);
    REQUIRE(b[0] == 42);
    REQUIRE(a.size() == 0);
    REQUIRE(a.capacity() == 0);
}

TEST_CASE("vector move assignment") {
    mystl::my_vector<int> a;
    a.push_back(100);

    mystl::my_vector<int> b;
    b = std::move(a);

    REQUIRE(b.size() == 1);
    REQUIRE(b[0] == 100);
    REQUIRE(a.size() == 0);
    REQUIRE(a.capacity() == 0);
}

TEST_CASE("vector pop_back throws on empty") {
    mystl::my_vector<int> v;
    REQUIRE_THROWS_AS(v.pop_back(), std::out_of_range);
}

TEST_CASE("vector push_back on empty vector") {
    mystl::my_vector<int> v;
    v.push_back(1);
    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 1);
}

TEST_CASE("vector push_back after reserve") {
    mystl::my_vector<int> v;
    v.reserve(10);  // Reserve space for 10 elements
    v.push_back(1);
    v.push_back(2);
    REQUIRE(v.size() == 2);
    REQUIRE(v.capacity() >= 10);  // Should not shrink after push_back
}

TEST_CASE("vector push_back after exceeding reserved capacity") {
    mystl::my_vector<int> v;
    v.reserve(5);  // Reserve space for 5 elements
    for (int i = 0; i < 10; ++i) {
        v.push_back(i);
    }
    REQUIRE(v.size() == 10);
    REQUIRE(v.capacity() > 5);  // The capacity should have grown automatically
}

TEST_CASE("vector access out of bounds") {
    mystl::my_vector<int> v;
    v.push_back(1);
    REQUIRE_THROWS_AS(v[1], std::out_of_range);  // Expecting out-of-bounds access
}

TEST_CASE("vector copy assignment empty to non-empty") {
    mystl::my_vector<int> a;
    a.push_back(1);
    mystl::my_vector<int> b;
    b = a;
    REQUIRE(b.size() == 1);
    REQUIRE(b[0] == 1);
}

TEST_CASE("vector clear resets size but not capacity") {
    mystl::my_vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.clear();
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() > 0);  // Capacity should remain the same after clear
}

TEST_CASE("vector large push_back") {
    mystl::my_vector<int> v;
    for (int i = 0; i < 1000000; ++i) {
        v.push_back(i);
    }
    REQUIRE(v.size() == 1000000);
    REQUIRE(v[999999] == 999999);  // Check last element
}

TEST_CASE("vector move constructor with large data") {
    mystl::my_vector<int> a;
    for (int i = 0; i < 100000; ++i) {
        a.push_back(i);
    }
    mystl::my_vector<int> b = std::move(a);
    REQUIRE(b.size() == 100000);
    REQUIRE(a.size() == 0);  // a should be empty after move
}

TEST_CASE("vector self assignment") {
    mystl::my_vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v = v;  // Self-assignment
    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == 10);
    REQUIRE(v[1] == 20);
}

TEST_CASE("vector destructor cleans up memory") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    // No explicit checks for memory leaks in tests, but you could use tools like Valgrind to check for memory leaks
}

TEST_CASE("vector deep copy constructor") {
    mystl::my_vector<int> a;
    for (int i = 0; i < 100000; ++i) {
        a.push_back(i);
    }

    mystl::my_vector<int> b = a;  // Copy constructor
    REQUIRE(b.size() == 100000);
    REQUIRE(b[99999] == 99999);  // Check last element

    // Ensure deep copy
    b[99999] = 123456;  // Modify one element
    REQUIRE(a[99999] != 123456);
}

TEST_CASE("vector equality operator") {
    mystl::my_vector<int> v1;
    mystl::my_vector<int> v2;
    REQUIRE(v1 == v2);  // Should be equal because both are empty
}

TEST_CASE("vector inequality operator") {
    mystl::my_vector<int> v1;
    mystl::my_vector<int> v2;
    v1.push_back(1);
    REQUIRE(v1 != v2);  // Should be unequal because v1 has an element
}

TEST_CASE("vector access after pop_back") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.pop_back();
    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 1);  // Valid access to the remaining element.
    REQUIRE_THROWS_AS(v[1], std::out_of_range);  // Out-of-bounds access should throw
}

TEST_CASE("vector reserve with smaller capacity") {
    mystl::my_vector<int> v;
    v.reserve(10);
    v.reserve(5);  // Trying to reduce capacity
    REQUIRE(v.capacity() == 10);  // Capacity should remain unchanged at 10
}

TEST_CASE("vector move constructor and assignment") {
    mystl::my_vector<int> v1;
    v1.push_back(10);
    v1.push_back(20);

    mystl::my_vector<int> v2 = std::move(v1);  // Move constructor
    REQUIRE(v2.size() == 2);
    REQUIRE(v2[0] == 10);
    REQUIRE(v2[1] == 20);
    REQUIRE(v1.size() == 0);  // v1 should be empty after move
    REQUIRE(v1.capacity() == 0);  // v1 should not hold any memory
}