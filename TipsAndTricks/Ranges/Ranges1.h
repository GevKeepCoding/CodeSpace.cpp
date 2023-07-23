#pragma once
import std;
//#include <ranges>
//#include <algorithm>
//#include <print>
//#include <sstream>
//#include <set>
//#include <numeric>
//#include <iostream>
//#include <vector>

using namespace std;
using namespace std::ranges;

namespace ranges_1 {

	void a1_1() {
		std::istringstream input("1 2 3 4 5 6 7");
		for (int i : views::istream<int>(input))
		{
			print("loop: {}\n", i);
		}
	}
	void a1_2() {
		std::istringstream input("1 2 3 4 5 6 7");
		for (int i : views::take(views::istream<int>(input), 3))
		{
			print("loop: {}\n", i);
		}
	}
	void a1_3() {
		std::istringstream input("1 2 3 4 5 6 7");
		for (int i : views::istream<int>(input)
			| views::take(5))
		{
			print("loop: {}\n", i);
		}
		int next;
		if (input >> next) {
			print("next: {}\n", next); // will print 7 ???
		}
	}

	// printing ranges
	void print_1() { // not working in current compiler
		// std::println("{}", std::vector{ 1,2,3 });
		//std::println("{:#>20}", std::vector{ 1,2,3 });
		//std::println("{:#>20:#x}", std::vector{ 1,2,3 });

		println("{:?}", "A\tCCU"s);
		//println("{:m}", std::pair{ "C++", 23 });
		//println("{:n}", std::set{ 1,2,3 });

		std::vector v = { 1,2,3,4,5 };
		//std::println("{}", v | std::views::chunk(2)); // [[1,2], [3,4],[5]]
		//std::println("{}", v | std::views::slide(2)); // [[1,2], [2,3],[3,4], [4,5]]
		//std::println("{}", v | std::views::stride(2)); // [1,3,5
	}
	namespace args {
		struct windowed {
			std::size_t size;
			std::size_t stride;
		};
	}
	constexpr auto windowed(args::windowed args)
	{
		using namespace std::views;
		return slide(args.size) | stride(args.stride);
	}
	void print_2() {
		//std::println("{}",
		//  std::views::iota(0, 20)
		//  | windowed({ .size = 5, .stride = 3 }));

		using namespace std::ranges;
		std::vector v = { 1,2,2,3,1,2,0,4,5,2 };
		//std::println("{}", v | views::chunk_by(less_equal{}));
		// we will get all monotonically increasing subsequences in a source code.
	}

	void Range_samples_1() {
		using namespace std::views;
		auto const nums = std::array{ 0,0,1,1,2 };
		auto const animals = std::array{ "cat"s, "dog"s };
		auto a0 = views::iota(0, 5) | chunk(2);
		auto a1 = nums | chunk_by(std::equal_to{});
		auto a2 = views::iota(0, 5) | slide(3);
		auto a3 = views::iota(0, 10) | stride(3);
		/*
		repeat(42) | take(5)
		zip_transform(std::plus{}, nums, nums);
		zip(iota(0,3), iota(1,4))				// [(0,1), (1,2), (2,3)]
		iota(0,4) | adjacent<2>					// [(0,1), (1,2), (2,3)]
		iota(0,4) | pairwise					// [(0,1), (1,2), (2,3)]
		iota(0,4) | adjacent_transform<2>(std::plus{}); // [1,3,5]
		iota(0,4) | pairwise_transform(std::plus{});	// [1,3,5]
		animals | join_with(',');				// ['c', 'a','t', ',' ,'d','o','g']
		cartesian_product(iota(0,2), "AZ"s)		// [(0,'A'), (0,'Z'), (1,'A'), (1,'Z')]
		enumerate("APL"s)						// [(0,'A'), (1,'P'), (2,'L')]
		*/


	}

	// simple tricks
	struct SiblingsInMusic
	{
		std::string sirName;
		std::string firstName;
		auto operator<=>(const SiblingsInMusic&) const = default;
	};
	void fun1() {
		std::vector<SiblingsInMusic> siblings = { {"Jackson", "Tito"}, {"Jackson", "Jackie"} };

		std::ranges::sort(siblings);
		std::ranges::for_each(siblings, [](const auto& sibling) {
			std::cout << sibling.sirName << " " << sibling.firstName << std::endl;
			});
	}


	void fun_Effective_Ranges_() {
		namespace rng = std::ranges;

		/*
		std::flat_map<std::string, int> fm;
		fm["world"] = 2;
		fm["hello"] = 1;
		std::print("{}\n", fm);
		for (auto [k, v] : rng::reverse_view{ fm })
		{
			std::print("{}:{}\n", k, v);
		}

		*/
		std::string path = "C:/file/bar/baz";
		auto leaf = rng::find_last(path, '/') | std::views::drop(1);
		std::print("{}\n", std::string_view(leaf));
		path = "none";
		leaf = rng::find_last(path, '/') | std::views::drop(1);
		auto size = leaf.size();
		std::print("empty: {}", std::string_view(leaf));

		// sort example
		struct stuff { int idx = 0; std::string s; };

		std::vector<stuff> vstuff = { {2,"foo"}, {1,"bar"}, {0,"baz"} };
		rng::sort(vstuff, std::less<>{},
				  [](auto const& e) { return e.idx; });




	}

}







//auto main() -> int {
//
//	ranges_1::print_1();
//	return 0;
//}