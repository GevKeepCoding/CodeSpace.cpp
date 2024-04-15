#pragma once
#include <random>
#include <ranges>

template<class Range>
auto enumerate(Range&& range) {
	return std::views::iota(0) | std::views::zip(std::forward<Range>(range));
}



#define REPEAT2(x)  {x} {x}
#define REPEAT4(x)  REPEAT2(x) REPEAT2(x)
#define REPEAT8(x)  REPEAT4(x) REPEAT4(x)
#define REPEAT16(x) REPEAT8(x) REPEAT8(x)
#define REPEAT32(x) REPEAT16(x) REPEAT16(x)    
#define REPEAT64(x) REPEAT32(x) REPEAT32(x)
#define REPEAT(x)   REPEAT64(x) REPEAT64(x)



template<std::integral T>
__declspec(noinline)
auto generate_random_integral_data(size_t size, unsigned int seed = 0) {
    std::mt19937 gen{seed};
    std::uniform_int_distribution<T> intDist{0, 1000};

    std::vector<T> data(size);
    std::generate(data.begin(), data.end(), [&]() { return static_cast<T>(intDist(gen)); });
    return data;
}

template<std::floating_point T>
__declspec(noinline)
auto generate_random_integral_data(size_t size, unsigned int seed = 0) {
	//std::random_device rd{};
	std::mt19937 gen{seed};
	std::uniform_real_distribution<T> intDist{0, 1000};

	std::vector<T> data(size);
	std::generate(data.begin(), data.end(), [&]() { return static_cast<T>(intDist(gen)); });
	return data;


}