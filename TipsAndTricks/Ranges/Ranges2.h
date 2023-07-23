#pragma once

import std;


namespace ranges2
{
	//Using the filter view in practice - Nicolai Josuttis - Meeting C++ 2023  https://www.youtube.com/watch?v=49ZYW4gHBIQ

	void print(const std::ranges::input_range auto& c) {
		for (const auto& elem : c) {
			std::cout << elem << ' ';
		}
		std::cout << '\n';
	}

	void print(std::ranges::input_range auto& c) {
		for (const auto& elem : c) {
			std::cout << elem << ' ';
		}
		std::cout << '\n';
	}



	using namespace std;
	using namespace std::ranges;


	int main_ranges2() {

		vector a{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		set b{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

		print(a);
		print(b);


		print(views::take(a, 3));
		print(views::take(b, 3));


		std::map<std::string, int> composers{
			{"Bach", 1685},
			{"Mozart", 1756},
			{"Beethoven", 1770},
			{"Tchaikovsky", 1840},
		};

		namespace v = std::ranges::views;
		for (const auto& elem : composers |
			 v::filter([](const auto& y) { return y.second > 1700; })
			 | v::take(3)
			 | v::keys) {
			cout << "- " << elem << '\n';
		}


		auto a1 = v::iota(1)
			| v::filter([](auto val) { return val % 3 == 0; })
			| v::drop(3)
			| v::take(8)
			| v::transform([](auto val) { return std::to_string(val) + "s"; });

		for (const auto& e : a1) {
			cout << e << "\n";
		}

		print(std::ranges::subrange{b | std::ranges::views::drop(2)});
		// should work but doesn't yet
		// print(std::ranges::subrange{a | std::ranges::views::filter([](auto val) { return val > 2; })});


		//---------------------------------------------------------------------
		// UB - ranges filter is broken
		// Example:
		{
			vector a = {1,4, 7, 10};
			auto isEven = [](auto&& i) { return i % 2 == 0; };
			auto collEven = a | std::ranges::views::filter(isEven);
			for (auto& i : collEven) {
				i += 1;
			}
			print(a);

			// increment even elements:
			for (auto& i : collEven) {
				i += 1;
			}
			print(a);
		}
		//---------------------------------------------------------------------
		{
			print("--------");
			vector vec{1,2,3,4};
			vec.reserve(9);
			list lst{1,2,3,4};

			auto vVec = vec | v::drop(2);
			auto vLst = lst | v::drop(2);

			// insert new elements at the front:
			vec.insert(vec.begin(), 0);
			lst.insert(lst.begin(), 0);

			print(vVec);
			print(vLst);

			print("--------");

			vec.insert(vec.begin(), {-2,-1});
			lst.insert(lst.begin(), {-2,-1});
			print(vVec);
			print(vLst);
			print("---------------");

			// copy of the view will drop caches
			auto vVec1 = vVec;
			auto vLst1 = vLst;
			print(vVec1);
			print(vLst1);
		}
		// filter is broken
		// need to remove caching
		// and fix const correctness
	}




}

