#pragma once
#include <ranges>
#include <random>
#include <list>
// write function that behaves like python enumerate using ranges
template <typename Range>
auto enumerate(Range&& range) {
    return std::views::iota(0) | std::views::zip(std::forward<Range>(range));
}

static std::random_device rd;
static std::mt19937 mt(rd());


std::vector<int> random_vector(int size, int min = 0, int max = 1000) {
    std::vector<int> res; res.reserve(size);
    std::uniform_int_distribution<int> dist(min, max);
    std::generate_n(res.begin(), size, [&]() { return dist(mt); });
    return res;
}


std::list<int> random_list(int size, int min = 0, int max = 1000) {
    std::list<int> res;
    std::uniform_int_distribution<int> dist(min, max);
    while (size--) {
		res.push_back(dist(mt));
    }
    return res;
}