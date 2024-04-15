#pragma once

#include <array>
#include <string>
#include <ranges>
#include <fmt/ranges.h>
#include <algorithm>
#include <numeric>
// ../cpp23_algos.png

namespace ranges_2
{
	using namespace std::views;
	using namespace std::string_literals;
	using namespace std::ranges;

	auto main() -> int {

		auto const nums = std::array{ 0,0,1,1,2,2 };
		auto const animals = std::array{ "cat"s, "dog"s };

		fmt::print("{}\n", std::views::iota(0, 5) | chunk(2));
		fmt::print("{}\n", nums | chunk_by(std::equal_to{}));
		fmt::print("{}\n", std::views::iota(0, 5) | slide(3));
		fmt::print("{}\n", std::views::iota(0, 10) | stride(3));
		fmt::print("{}\n", repeat(42) | take(5));
		fmt::print("{}\n", zip_transform(std::plus{}, nums, nums));
		fmt::print("{}\n", std::ranges::views::zip(std::views::iota(0, 3), std::views::iota(1, 4)));
		fmt::print("{}\n", std::views::iota(0, 4) | adjacent<2>);
		fmt::print("{}\n", std::views::iota(0, 4) | pairwise);
		fmt::print("{}\n", std::views::iota(0, 4) | adjacent_transform<2>(std::plus{}));
		fmt::print("{}\n", std::views::iota(0, 4) | pairwise_transform(std::plus{}));
		fmt::print("{}\n", animals | join_with(','));
		fmt::print("{}\n", cartesian_product(std::views::iota(0, 2), "AZ"s));
		fmt::print("{}\n", enumerate("APL"s));

		return 0;
	}

}
