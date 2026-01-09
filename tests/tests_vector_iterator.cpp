#include "../source/vector.hpp"
#include "../extras/catch_amalgamated.hpp"
#include <algorithm>  // For std algorithms
#include <numeric>    // For std::accumulate

// ============================================================
// BASIC ITERATOR TESTS
// These tests were formulated with the help of AI
// ============================================================

TEST_CASE("iterator - basic begin/end") {
    mystl::my_vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    
    // Test that begin() points to first element
    REQUIRE(*v.begin() == 10);
    
    // Test that end() is one past last element
    auto it = v.begin();
    ++it;  // Points to 20
    ++it;  // Points to 30
    ++it;  // Now equals end()
    REQUIRE(it == v.end());
}

TEST_CASE("iterator - traversal with pointer arithmetic") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    // Method 1: Using iterator increment
    auto it = v.begin();
    REQUIRE(*it == 1);
    ++it;
    REQUIRE(*it == 2);
    ++it;
    REQUIRE(*it == 3);
    ++it;
    REQUIRE(it == v.end());
    
    // Method 2: Using pointer arithmetic (random access)
    REQUIRE(*(v.begin() + 0) == 1);
    REQUIRE(*(v.begin() + 1) == 2);
    REQUIRE(*(v.begin() + 2) == 3);
    
    // Method 3: Using array-style indexing on iterator
    auto start = v.begin();
    REQUIRE(start[0] == 1);
    REQUIRE(start[1] == 2);
    REQUIRE(start[2] == 3);
}

TEST_CASE("iterator - modification through iterator") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    // Modify elements through iterator
    for(auto it = v.begin(); it != v.end(); ++it) {
        *it *= 10;  // Dereference and modify
    }
    
    REQUIRE(v[0] == 10);
    REQUIRE(v[1] == 20);
    REQUIRE(v[2] == 30);
}

// ============================================================
// CONST ITERATOR TESTS
// ============================================================

TEST_CASE("const_iterator - read-only access") {
    mystl::my_vector<int> v;
    v.push_back(5);
    v.push_back(10);
    
    // Using const_iterator prevents modification
    int sum = 0;
    for(auto it = v.cbegin(); it != v.cend(); ++it) {
        sum += *it;
        // *it = 100;  // This would cause compilation error!
    }
    REQUIRE(sum == 15);
}

TEST_CASE("const_iterator - on const vector") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    
    const auto& const_v = v;
    
    // On const vector, begin() returns const_iterator
    auto it = const_v.begin();
    REQUIRE(*it == 1);
    // *it = 100;  // Compilation error!
}

// ============================================================
// RANGE-BASED FOR LOOP TESTS
// ============================================================

TEST_CASE("range-based for loop - mutable") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    // Range-based for uses begin()/end() internally
    for(auto& element : v) {
        element *= 2;
    }
    
    REQUIRE(v[0] == 2);
    REQUIRE(v[1] == 4);
    REQUIRE(v[2] == 6);
}

TEST_CASE("range-based for loop - const") {
    mystl::my_vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    
    int sum = 0;
    for(const auto& element : v) {
        sum += element;
    }
    
    REQUIRE(sum == 60);
}

// ============================================================
// STL ALGORITHM COMPATIBILITY TESTS
// ============================================================

TEST_CASE("STL algorithms - std::find") {
    mystl::my_vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    
    // std::find returns iterator to found element
    auto it = std::find(v.begin(), v.end(), 20);
    REQUIRE(it != v.end());
    REQUIRE(*it == 20);
    
    // Search for non-existent element
    auto not_found = std::find(v.begin(), v.end(), 99);
    REQUIRE(not_found == v.end());
}

TEST_CASE("STL algorithms - std::count") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(2);
    v.push_back(3);
    v.push_back(2);
    
    auto count = std::count(v.begin(), v.end(), 2);
    REQUIRE(count == 3);
}

TEST_CASE("STL algorithms - std::accumulate") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    
    // Sum all elements
    int sum = std::accumulate(v.begin(), v.end(), 0);
    REQUIRE(sum == 10);
}

TEST_CASE("STL algorithms - std::sort") {
    mystl::my_vector<int> v;
    v.push_back(30);
    v.push_back(10);
    v.push_back(40);
    v.push_back(20);
    
    // Sort in place
    std::sort(v.begin(), v.end());
    
    REQUIRE(v[0] == 10);
    REQUIRE(v[1] == 20);
    REQUIRE(v[2] == 30);
    REQUIRE(v[3] == 40);
}

TEST_CASE("STL algorithms - std::reverse") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    std::reverse(v.begin(), v.end());
    
    REQUIRE(v[0] == 3);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 1);
}

TEST_CASE("STL algorithms - std::for_each") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    // Apply function to each element
    int sum = 0;
    std::for_each(v.begin(), v.end(), [&sum](int val) {
        sum += val;
    });
    
    REQUIRE(sum == 6);
}

// ============================================================
// REVERSE ITERATOR TESTS
// ============================================================

TEST_CASE("reverse_iterator - basic traversal") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    // Traverse backwards
    auto rit = v.rbegin();
    REQUIRE(*rit == 3);
    ++rit;
    REQUIRE(*rit == 2);
    ++rit;
    REQUIRE(*rit == 1);
    ++rit;
    REQUIRE(rit == v.rend());
}

TEST_CASE("reverse_iterator - range-based (C++20 style)") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    std::vector<int> result;
    for(auto it = v.rbegin(); it != v.rend(); ++it) {
        result.push_back(*it);
    }
    
    REQUIRE(result[0] == 3);
    REQUIRE(result[1] == 2);
    REQUIRE(result[2] == 1);
}

// ============================================================
// DISTANCE AND ADVANCE TESTS
// ============================================================

TEST_CASE("iterator distance") {
    mystl::my_vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    v.push_back(40);
    
    // Calculate distance between iterators
    auto distance = std::distance(v.begin(), v.end());
    REQUIRE(distance == 4);
    
    auto it = v.begin();
    ++it;
    ++it;
    distance = std::distance(v.begin(), it);
    REQUIRE(distance == 2);
}

TEST_CASE("iterator advance") {
    mystl::my_vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    v.push_back(40);
    
    auto it = v.begin();
    std::advance(it, 2);  // Move iterator forward by 2
    REQUIRE(*it == 30);
}

// ============================================================
// EMPTY VECTOR TESTS
// ============================================================

TEST_CASE("iterator - empty vector") {
    mystl::my_vector<int> v;
    
    // For empty vector, begin() == end()
    REQUIRE(v.begin() == v.end());
    
    // This loop won't execute
    int count = 0;
    for(auto it = v.begin(); it != v.end(); ++it) {
        count++;
    }
    REQUIRE(count == 0);
}

// ============================================================
// PRACTICAL EXAMPLE: Custom Predicate
// ============================================================

TEST_CASE("practical example - filter even numbers") {
    mystl::my_vector<int> v;
    for(int i = 1; i <= 10; i++) {
        v.push_back(i);
    }
    
    // Count even numbers using std::count_if
    auto even_count = std::count_if(v.begin(), v.end(), 
        [](int x) { return x % 2 == 0; });
    
    REQUIRE(even_count == 5);
}

TEST_CASE("practical example - find maximum") {
    mystl::my_vector<int> v;
    v.push_back(10);
    v.push_back(50);
    v.push_back(30);
    v.push_back(20);
    
    // Find iterator to maximum element
    auto max_it = std::max_element(v.begin(), v.end());
    REQUIRE(*max_it == 50);
}

// ============================================================
// DEMONSTRATION: Why Iterators Are Powerful
// ============================================================

TEST_CASE("demonstration - generic algorithm") {
    mystl::my_vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    
    // Same algorithm works with std::vector too!
    std::vector<int> v2 = {1, 2, 3};
    
    int sum1 = std::accumulate(v1.begin(), v1.end(), 0);
    int sum2 = std::accumulate(v2.begin(), v2.end(), 0);
    
    REQUIRE(sum1 == sum2);
    REQUIRE(sum1 == 6);
}

// ============================================================
// EDGE CASE TESTS - EMPTY VECTOR
// ============================================================

TEST_CASE("edge case - empty vector iterators equal") {
    mystl::my_vector<int> v;
    REQUIRE(v.begin() == v.end());
    REQUIRE(v.cbegin() == v.cend());
    REQUIRE(v.rbegin() == v.rend());
    REQUIRE(v.crbegin() == v.crend());
}

TEST_CASE("edge case - empty vector iterator distance") {
    mystl::my_vector<int> v;
    REQUIRE(std::distance(v.begin(), v.end()) == 0);
    REQUIRE(std::distance(v.rbegin(), v.rend()) == 0);
}

TEST_CASE("edge case - empty vector with algorithms") {
    mystl::my_vector<int> v;
    
    // std::find on empty vector should return end()
    auto it = std::find(v.begin(), v.end(), 42);
    REQUIRE(it == v.end());
    
    // std::count on empty vector should return 0
    auto count = std::count(v.begin(), v.end(), 42);
    REQUIRE(count == 0);
    
    // std::accumulate on empty vector should return initial value
    auto sum = std::accumulate(v.begin(), v.end(), 100);
    REQUIRE(sum == 100);
}

// ============================================================
// EDGE CASE TESTS - SINGLE ELEMENT
// ============================================================

TEST_CASE("edge case - single element vector") {
    mystl::my_vector<int> v;
    v.push_back(42);
    
    REQUIRE(v.begin() != v.end());
    REQUIRE(*v.begin() == 42);
    
    auto it = v.begin();
    ++it;
    REQUIRE(it == v.end());
}

TEST_CASE("edge case - single element reverse iteration") {
    mystl::my_vector<int> v;
    v.push_back(99);
    
    auto rit = v.rbegin();
    REQUIRE(*rit == 99);
    ++rit;
    REQUIRE(rit == v.rend());
}

// ============================================================
// EDGE CASE TESTS - ITERATOR INVALIDATION
// ============================================================

TEST_CASE("edge case - iterator after push_back may invalidate") {
    mystl::my_vector<int> v;
    v.reserve(2);  // Reserve exactly 2 elements
    v.push_back(1);
    v.push_back(2);
    
    auto it = v.begin();
    REQUIRE(*it == 1);
    
    // This push_back will trigger reallocation, potentially invalidating 'it'
    v.push_back(3);
    
    // Note: After reallocation, old iterators are invalid
    // We need to get a fresh iterator
    auto new_it = v.begin();
    REQUIRE(*new_it == 1);
    REQUIRE(v.size() == 3);
}

TEST_CASE("edge case - iterator after pop_back") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    auto it = v.begin();
    std::advance(it, 2);  // Points to element at index 2 (value 3)
    REQUIRE(*it == 3);
    
    v.pop_back();  // Removes element at index 2
    
    // 'it' now points to one-past-end, which is undefined behavior to dereference
    // We can only check that it equals end()
    REQUIRE(it == v.end());
}

TEST_CASE("edge case - iterator after clear") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    
    auto it = v.begin();
    v.clear();
    
    // After clear, old iterators are invalid
    // begin() should equal end() for empty vector
    REQUIRE(v.begin() == v.end());
    REQUIRE(v.size() == 0);
}

// ============================================================
// EDGE CASE TESTS - ITERATOR ARITHMETIC
// ============================================================

TEST_CASE("edge case - iterator arithmetic with zero") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    auto it = v.begin();
    auto same_it = it + 0;
    REQUIRE(it == same_it);
    REQUIRE(*same_it == 1);
}

TEST_CASE("edge case - iterator arithmetic to end") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    auto it = v.begin() + 3;
    REQUIRE(it == v.end());
}

TEST_CASE("edge case - iterator subtraction") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    auto diff = v.end() - v.begin();
    REQUIRE(diff == 3);
    
    auto it1 = v.begin();
    auto it2 = v.begin() + 2;
    REQUIRE(it2 - it1 == 2);
}

// ============================================================
// EDGE CASE TESTS - REVERSE ITERATORS
// ============================================================

TEST_CASE("edge case - reverse iterator on single element") {
    mystl::my_vector<int> v;
    v.push_back(42);
    
    auto rit = v.rbegin();
    REQUIRE(*rit == 42);
    ++rit;
    REQUIRE(rit == v.rend());
}

TEST_CASE("edge case - reverse iterator distance") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    
    auto dist = std::distance(v.rbegin(), v.rend());
    REQUIRE(dist == 4);
}

TEST_CASE("edge case - mixing forward and reverse iterators") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    // Forward iteration
    std::vector<int> forward_result;
    for(auto it = v.begin(); it != v.end(); ++it) {
        forward_result.push_back(*it);
    }
    
    // Reverse iteration
    std::vector<int> reverse_result;
    for(auto rit = v.rbegin(); rit != v.rend(); ++rit) {
        reverse_result.push_back(*rit);
    }
    
    // Results should be mirrors of each other
    REQUIRE(forward_result[0] == reverse_result[2]);
    REQUIRE(forward_result[1] == reverse_result[1]);
    REQUIRE(forward_result[2] == reverse_result[0]);
}

// ============================================================
// STRESS TESTS - LARGE DATA
// ============================================================

TEST_CASE("stress test - large vector iteration") {
    mystl::my_vector<int> v;
    const size_t large_size = 10000;
    
    for(size_t i = 0; i < large_size; i++) {
        v.push_back(static_cast<int>(i));
    }
    
    // Forward iteration
    size_t count = 0;
    for(auto it = v.begin(); it != v.end(); ++it) {
        REQUIRE(*it == static_cast<int>(count));
        count++;
    }
    REQUIRE(count == large_size);
    
    // Reverse iteration
    count = large_size;
    for(auto rit = v.rbegin(); rit != v.rend(); ++rit) {
        count--;
        REQUIRE(*rit == static_cast<int>(count));
    }
    REQUIRE(count == 0);
}

TEST_CASE("stress test - iterator with frequent modifications") {
    mystl::my_vector<int> v;
    
    // Add many elements
    for(int i = 0; i < 1000; i++) {
        v.push_back(i);
    }
    
    // Verify all elements
    int expected = 0;
    for(auto it = v.begin(); it != v.end(); ++it, ++expected) {
        REQUIRE(*it == expected);
    }
}

TEST_CASE("stress test - STL algorithm on large vector") {
    mystl::my_vector<int> v;
    
    for(int i = 1; i <= 1000; i++) {
        v.push_back(i);
    }
    
    // Test std::find on last element (worst case)
    auto it = std::find(v.begin(), v.end(), 1000);
    REQUIRE(it != v.end());
    REQUIRE(*it == 1000);
    
    // Test std::accumulate
    int sum = std::accumulate(v.begin(), v.end(), 0);
    REQUIRE(sum == 500500);  // Sum of 1 to 1000
}

// ============================================================
// CONST CORRECTNESS TESTS
// ============================================================

TEST_CASE("const correctness - const_iterator from const vector") {
    const mystl::my_vector<int> v = []() {
        mystl::my_vector<int> temp;
        temp.push_back(1);
        temp.push_back(2);
        temp.push_back(3);
        return temp;
    }();
    
    int sum = 0;
    for(auto it = v.begin(); it != v.end(); ++it) {
        sum += *it;
    }
    REQUIRE(sum == 6);
}

TEST_CASE("const correctness - cbegin/cend on mutable vector") {
    mystl::my_vector<int> v;
    v.push_back(10);
    v.push_back(20);
    
    // cbegin()/cend() should return const_iterator even on non-const vector
    auto it = v.cbegin();
    REQUIRE(*it == 10);
    // *it = 100;  // Should not compile
}

// ============================================================
// ALGORITHM COMPATIBILITY EDGE CASES
// ============================================================

TEST_CASE("algorithm edge case - std::find_if with no match") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(3);
    v.push_back(5);
    
    auto it = std::find_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
    REQUIRE(it == v.end());
}

TEST_CASE("algorithm edge case - std::all_of on empty") {
    mystl::my_vector<int> v;
    
    // std::all_of returns true for empty range (vacuous truth)
    bool result = std::all_of(v.begin(), v.end(), [](int x) { return x > 0; });
    REQUIRE(result == true);
}

TEST_CASE("algorithm edge case - std::none_of") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(3);
    v.push_back(5);
    
    bool result = std::none_of(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
    REQUIRE(result == true);
}

TEST_CASE("algorithm edge case - std::min_element on single element") {
    mystl::my_vector<int> v;
    v.push_back(42);
    
    auto it = std::min_element(v.begin(), v.end());
    REQUIRE(it != v.end());
    REQUIRE(*it == 42);
}

// ============================================================
// POTENTIAL BUG TESTS - BOUNDARY CONDITIONS
// ============================================================

TEST_CASE("boundary - dereferencing begin on empty (should not crash in tests)") {
    mystl::my_vector<int> v;
    // Don't actually dereference - this would be UB
    // Just verify iterators are equal
    REQUIRE(v.begin() == v.end());
}

TEST_CASE("boundary - iterator equality after multiple increments") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    
    auto it1 = v.begin();
    auto it2 = v.begin();
    
    ++it1;
    ++it2;
    
    REQUIRE(it1 == it2);
    REQUIRE(*it1 == *it2);
}

TEST_CASE("boundary - const and non-const iterator comparison") {
    mystl::my_vector<int> v;
    v.push_back(1);
    
    auto it = v.begin();
    auto cit = v.cbegin();
    
    // These should be comparable (implicit conversion)
    REQUIRE(it == cit);
}

TEST_CASE("boundary - reverse iterator base") {
    mystl::my_vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    auto rit = v.rbegin();
    ++rit;  // Points to second-to-last element (2)
    
    // .base() returns the underlying forward iterator
    auto base = rit.base();
    // base points one position ahead in forward iteration
    --base;
    REQUIRE(*base == 2);
}