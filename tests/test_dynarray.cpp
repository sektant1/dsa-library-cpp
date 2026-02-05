// dynarray_mit_tests.cpp
// Catch2 v3 test suite for your DynArray interface (MIT-style dynamic sequence checks)
//
// Assumes your header is named: DynArray.h
//
// Build (example):
//   g++ -std=c++20 -O0 -g -I/path/to/catch2 -I. dynarray_mit_tests.cpp -o tests
//   ./tests
//
// Notes:
// - These tests are written to match the *interface you provided*:
//   size(), capacity(), empty(), operator[], front/back, begin/end, resize/reserve,
//   push_back/pop_back/clear, copy/move.
// - Several tests will likely FAIL with your current implementation (that’s good: they reveal bugs).

#define CATCH_CONFIG_MAIN
#include <algorithm>
#include <numeric>
#include <string>
#include <type_traits>

#include <catch2/catch_all.hpp>

#include "dsa/DynArray.hpp"

template<class A>
static A make_ints(std::initializer_list<int> xs)
{
    A a;
    for (int x : xs) {
        a.push_back(x);
    }
    return a;
}

TEST_CASE("MIT sequence invariants: default construction", "[dynarray][mit]")
{
    DynArray<int> a;

    REQUIRE(a.size() == 0);
    REQUIRE(a.empty() == true);
    REQUIRE(a.capacity() >= a.size());

    // iterator invariants for empty container
    REQUIRE(a.begin() == a.end());
}

TEST_CASE("MIT sequence: push_back grows size, preserves order, front/back", "[dynarray][mit]")
{
    DynArray<int> a;
    a.push_back(10);
    a.push_back(20);
    a.push_back(30);

    REQUIRE(a.size() == 3);
    REQUIRE(a.empty() == false);

    REQUIRE(a.front() == 10);
    REQUIRE(a.back() == 30);

    // get() via const operator[]
    const DynArray<int> &ca = a;
    REQUIRE(ca[0] == 10);
    REQUIRE(ca[1] == 20);
    REQUIRE(ca[2] == 30);
}

TEST_CASE("MIT sequence: set/get via operator[] reference semantics", "[dynarray][mit]")
{
    DynArray<int> a;
    a.push_back(1);
    a.push_back(2);

    // set(): non-const operator[] should return T&
    a[0] = 42;
    REQUIRE(a.front() == 42);

    // get(): const operator[] should return const T&
    const DynArray<int> &ca = a;
    REQUIRE(ca[0] == 42);

    // Ensure operator[] really returns a reference (compile-time check)
    STATIC_REQUIRE(std::is_same_v<decltype(a[0]), int &>);
    STATIC_REQUIRE(std::is_same_v<decltype(ca[0]), const int &>);
}

TEST_CASE("MIT sequence: pop_back removes last item and updates back()", "[dynarray][mit]")
{
    DynArray<int> a = make_ints<DynArray<int>>({1, 2, 3});

    REQUIRE(a.size() == 3);
    REQUIRE(a.back() == 3);

    a.pop_back();
    REQUIRE(a.size() == 2);
    REQUIRE(a.back() == 2);

    a.pop_back();
    REQUIRE(a.size() == 1);
    REQUIRE(a.back() == 1);

    a.pop_back();
    REQUIRE(a.size() == 0);
    REQUIRE(a.empty());
}

TEST_CASE("MIT sequence: begin/end iterate all elements in order", "[dynarray][mit]")
{
    DynArray<int> a = make_ints<DynArray<int>>({5, 6, 7, 8});

    int sum = 0;
    for (auto it = a.begin(); it != a.end(); ++it) {
        sum += *it;
    }
    REQUIRE(sum == (5 + 6 + 7 + 8));

    // range-for should also work (uses begin/end)
    int prod = 1;
    for (int x : a) {
        prod *= x;
    }
    REQUIRE(prod == 5 * 6 * 7 * 8);
}

TEST_CASE("MIT sequence: const iteration is read-only", "[dynarray][mit]")
{
    DynArray<int>        a  = make_ints<DynArray<int>>({1, 2, 3});
    const DynArray<int> &ca = a;

    // Can read through const iterators
    int sum = 0;
    for (auto it = ca.begin(); it != ca.end(); ++it) {
        sum += *it;
        // *it = 5; // should NOT compile (const_iterator is const T*)
    }
    REQUIRE(sum == 6);

    STATIC_REQUIRE(std::is_same_v<decltype(ca.begin()), DynArray<int>::const_iterator>);
    STATIC_REQUIRE(std::is_same_v<decltype(a.begin()), DynArray<int>::iterator>);
}

TEST_CASE("MIT dynamic array: reserve increases capacity and preserves elements", "[dynarray][mit]")
{
    DynArray<int> a       = make_ints<DynArray<int>>({10, 20, 30, 40});
    const int     oldSize = a.size();
    const int     oldCap  = a.capacity();

    a.reserve(oldCap + 50);

    REQUIRE(a.size() == oldSize);
    REQUIRE(a.capacity() >= oldCap + 50);

    for (int i = 0; i < oldSize; ++i) {
        REQUIRE(a[i] == (i + 1) * 10);
    }
}

TEST_CASE("MIT dynamic array: resize grows/shrinks size, preserves prefix", "[dynarray][mit]")
{
    DynArray<int> a      = make_ints<DynArray<int>>({1, 2, 3, 4});
    const int     oldCap = a.capacity();

    SECTION("shrink")
    {
        a.resize(2);
        REQUIRE(a.size() == 2);
        REQUIRE(a[0] == 1);
        REQUIRE(a[1] == 2);
        REQUIRE(a.capacity() >= a.size());
    }

    SECTION("grow")
    {
        a.resize(10);
        REQUIRE(a.size() == 10);
        REQUIRE(a.capacity() >= 10);
        // Prefix should be preserved
        REQUIRE(a[0] == 1);
        REQUIRE(a[1] == 2);
        REQUIRE(a[2] == 3);
        REQUIRE(a[3] == 4);

        // capacity should not shrink when growing; usually >= oldCap
        REQUIRE(a.capacity() >= oldCap);
    }
}

TEST_CASE("MIT: clear sets size to 0 but container remains usable", "[dynarray][mit]")
{
    DynArray<std::string> a;
    a.push_back("a");
    a.push_back("b");
    a.push_back("c");

    REQUIRE(a.size() == 3);
    a.clear();

    REQUIRE(a.size() == 0);
    REQUIRE(a.empty());
    REQUIRE(a.begin() == a.end());

    // Should be reusable after clear
    a.push_back("x");
    a.push_back("y");
    REQUIRE(a.size() == 2);
    REQUIRE(a.front() == "x");
    REQUIRE(a.back() == "y");
}

TEST_CASE("MIT: copy ctor/assignment produce independent copies (deep copy)", "[dynarray][mit]")
{
    DynArray<int> a = make_ints<DynArray<int>>({1, 2, 3});

    SECTION("copy constructor")
    {
        DynArray<int> b(a);
        REQUIRE(b.size() == 3);
        REQUIRE(b[0] == 1);
        REQUIRE(b[1] == 2);
        REQUIRE(b[2] == 3);

        a[0] = 99;
        REQUIRE(b[0] == 1);  // should not change if deep copy
    }

    SECTION("copy assignment")
    {
        DynArray<int> b;
        b.push_back(7);
        b.push_back(8);

        b = a;
        REQUIRE(b.size() == 3);
        REQUIRE(b[0] == 1);
        REQUIRE(b[1] == 2);
        REQUIRE(b[2] == 3);

        a[1] = 55;
        REQUIRE(b[1] == 2);
    }
}

TEST_CASE("MIT: move ctor/assignment leaves moved-to with data and moved-from valid", "[dynarray][mit]")
{
    DynArray<int> a = make_ints<DynArray<int>>({1, 2, 3});

    SECTION("move constructor")
    {
        DynArray<int> b(std::move(a));
        REQUIRE(b.size() == 3);
        REQUIRE(b[0] == 1);
        REQUIRE(b[1] == 2);
        REQUIRE(b[2] == 3);

        // moved-from should be valid to destroy / assign / push
        REQUIRE(a.size() == 0);
        REQUIRE(a.capacity() == 0);
        REQUIRE(a.begin() == a.end());

        a.push_back(9);
        REQUIRE(a.size() == 1);
        REQUIRE(a.front() == 9);
    }

    SECTION("move assignment")
    {
        DynArray<int> b;
        b.push_back(100);

        b = std::move(a);
        REQUIRE(b.size() == 3);
        REQUIRE(b[0] == 1);
        REQUIRE(b[1] == 2);
        REQUIRE(b[2] == 3);

        REQUIRE(a.size() == 0);
        REQUIRE(a.capacity() == 0);
    }
}

TEST_CASE("MIT: repeated push_back triggers growth and keeps all values", "[dynarray][mit]")
{
    DynArray<int> a;
    const int     N = 200;

    for (int i = 0; i < N; ++i) {
        a.push_back(i);
    }

    REQUIRE(a.size() == N);
    REQUIRE(a.capacity() >= N);

    for (int i = 0; i < N; ++i) {
        REQUIRE(a[i] == i);
    }
}
