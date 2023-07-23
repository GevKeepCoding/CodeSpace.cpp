#pragma once
#include <mdspan>
#include <vector>
#include <print>
#include <algorithm>
#include <numeric>
#include <span>


using namespace std;



// mdspan is c++23 feature and isn't yet implemented fully by Microsoft compiler
//void Example1() {
//		std::vector v = { 1,2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
//		auto ms2 = std::mdspan(v.data(), 2, 6); //2 rows 6 element each
//		auto ms3 = mdspan(v.data(), 2, 3, 2);// 2 x 3 x 2
//
//		// iterate over all elements of ms2
//		for (auto e : ms2)
//			std::printf("%d ", e);
//
//
//		for (size_t i = 0; i != ms2.extent(0); i++)
//			for (size_t j = 0; j != ms2.extent(1); j++)
//				ms2 [i, j] = i * 100 + j;
//
//		for (size_t i = 0; i != ms3.extent(0); i++)
//		{
//			std::println("slice @ i = {}", i);
//			for (size_t j = 0; j < ms3.extent(1); j++)
//			{
//				for (size_t k = 0; k < ms3.extent(2); k++)
//					std::printf("{}", ms3[i, j, k]);
//				std::println("");
//			}
//		}
//
//	}
// write a function that will add two msdpan's together
// and return the result
namespace mdspan_1 {

	void Example2_extents() {
		std::extents e0{ 16,32 };
		// Equivalent to 
		std::extents<size_t, std::dynamic_extent, std::dynamic_extent> e1{ 16,32 };

		std::println("{} {} {}", e0.rank(), e0.extent(0), e0.extent(1));

		//---
		std::dextents<int, 2> e2{ 16,32 };

		std::extents<size_t, 16, 32> e3;
		std::extents<size_t, 16> e4{ 32 };

		std::extents e5{ 16,32 };
	}

	void Example2() {
		std::vector v = { 1,2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
		mdspan m0{ v.data(), 16,32 };
		// same as
		std::mdspan<int, std::dextents<int, 2> > m1{ v.data(), 16,32 };
		int i, j;
		//m0[i, j] == v[i * 16 + j];
		std::mdspan m2{ v.data(), std::extents<int, 16,32>{} };
		std::mdspan<int, std::extents<int, 16, 32>> m3{ v.data() };

		std::mdspan m4{ v.data(), std::extents<int, 16, std::dynamic_extent>{32} };

	}

}
int main() {
	using namespace mdspan_1;

	Example2_extents();


}