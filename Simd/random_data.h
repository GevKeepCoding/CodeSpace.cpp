#pragma once
#include <random>


namespace random_data {

	

	template<class T>
	__declspec(noinline)
	auto generate_random_data(size_t size) {
		std::random_device rd{};
		std::mt19937 gen{rd()};
		std::uniform_int_distribution intDist{0, 1000};

		std::vector<T> data(size);
		std::generate(data.begin(), data.end(), [&]() { return static_cast<T>(intDist(gen)); });
		return data;
	}
}