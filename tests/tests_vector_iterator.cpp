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