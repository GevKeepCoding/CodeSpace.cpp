#pragma once



#include <iostream>
#include <vector>
#include <ranges>

int main() {
	std::vector<int> data{1, 2, 3, 4, 5, 6, 7};

	for (auto [index, value] : data | std::views::enumerate) {
		std::cout << "Index: " << index << ", Value: " << value << std::endl;
	}
	std::cout << "-------------------------------";


	
	
	
	// Applying enumerate before other views to inject original indexes
	constexpr auto before = std::views::enumerate | std::views::filter([](auto t) {
		return std::get<1>(t) % 2 == 0;
	});

	for (const auto& [index, value] : data | before) {
		std::cout << "Index: " << index << ", Value: " << value << std::endl;
	}

	return 0;
}
