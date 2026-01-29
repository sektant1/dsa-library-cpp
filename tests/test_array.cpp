#include <catch2/catch_all.hpp>

#include "dsa/array.hpp"

using dsa::Array;

TEST_CASE("Array behaves correctly", "[Array]")
{
    Array<int> arr;
    // REQUIRE(arr.size() == 0);
    //
    // arr.push_back(10);
    // arr.push_back(20);
    //
    // REQUIRE(arr.size() == 2);
    // REQUIRE(arr[0] == 10);
    // REQUIRE(arr[1] == 20);
}
