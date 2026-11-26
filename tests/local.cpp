#include <cassert>
#include <iostream>
#include <numeric>
#include <random>
#include <set>
#include "../source.h"

static std::vector<int> hidden;
static int calls;

int query(int x, int y, int z) {
    assert(x != y && x != z && y != z);
    assert(x >= 1 && y >= 1 && z >= 1);
    assert(x <= int(hidden.size()) && y <= int(hidden.size()) && z <= int(hidden.size()));
    ++calls;
    return std::min({hidden[x - 1], hidden[y - 1], hidden[z - 1]})
         + std::max({hidden[x - 1], hidden[y - 1], hidden[z - 1]});
}

static void check(const std::vector<int>& input) {
    hidden = input;
    calls = 0;
    long long expected = 0, power = 1;
    for (int value : hidden) {
        power = power * 233 % 998244353;
        expected = (expected + value * power) % 998244353;
    }
    assert(guess(int(hidden.size()), 5) == expected);
    assert(calls <= std::min(int(hidden.size()) + 35, 2 * int(hidden.size())));
}

int main() {
    for (int mask = 0; mask < (1 << 9); ++mask) {
        if (__builtin_popcount(unsigned(mask)) != 5) continue;
        std::vector<int> input;
        for (int i = 0; i < 9; ++i) if (mask & (1 << i)) input.push_back(i + 1);
        do { check(input); } while (std::next_permutation(input.begin(), input.end()));
    }
    std::mt19937 rng(1936);
    for (int test = 0; test < 2000; ++test) {
        int n = test < 20 ? 10000 : 5 + rng() % 200;
        std::set<int> used;
        while (int(used.size()) < n) used.insert(1 + rng() % 1000000000);
        std::vector<int> input(used.begin(), used.end());
        std::shuffle(input.begin(), input.end(), rng);
        check(input);
    }
    std::vector<int> nested = {1, 250000000, 500000000, 750000000, 1000000000};
    int endpoint = 250000000;
    while (endpoint > 2) {
        endpoint = (1 + endpoint) / 2;
        nested.push_back(endpoint);
    }
    check(nested);
    std::cout << "All exhaustive, random, and nested-interval tests passed.\n";
}
