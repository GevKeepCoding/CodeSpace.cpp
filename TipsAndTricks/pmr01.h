#pragma once
#include <memory_resource>


#include <array>
#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <list>
#include <memory_resource>
#include <unordered_map>
#include <algorithm>
#include <ranges>

namespace mypmr01 {





	void do_work(std::pmr::polymorphic_allocator<> alloc) {
		auto p = alloc.allocate(1);
		alloc.deallocate(p, 1);
	}

	void func01() {
		auto heap = std::pmr::new_delete_resource();
		std::pmr::monotonic_buffer_resource monotonic_buffers(heap);
		std::pmr::unsynchronized_pool_resource pool(&monotonic_buffers);
		do_work({ &pool });
	}



	template<typename Func>
	auto benchmark(Func test_func, int iterations)
	{
		const auto start = std::chrono::system_clock::now();
		while (iterations-- > 0)
			test_func();
		const auto stop = std::chrono::system_clock::now();
		const auto secs = std::chrono::duration<double>(stop - start);
		return secs.count();
	}

	int main_pmr01()
	{
		constexpr int iterations{ 100 };
		constexpr int total_nodes{ 2'00'000 };

		auto default_std_alloc = [total_nodes]
		{
			std::list<int> list;
			for (int i{}; i != total_nodes; ++i)
				list.push_back(i);
		};

		auto default_pmr_alloc = [total_nodes]
		{
			std::pmr::list<int> list;
			for (int i{}; i != total_nodes; ++i)
				list.push_back(i);
		};

		auto pmr_alloc_no_buf = [total_nodes]
		{
			std::pmr::monotonic_buffer_resource mbr;
			std::pmr::polymorphic_allocator<int> pa{ &mbr };
			std::pmr::list<int> list{ pa };
			for (int i{}; i != total_nodes; ++i)
				list.push_back(i);
		};

		auto pmr_alloc_and_buf = [total_nodes]
		{
			std::array<std::byte, total_nodes * 32> buffer; // enough to fit in all nodes
			std::pmr::monotonic_buffer_resource mbr{ buffer.data(), buffer.size() };
			std::pmr::polymorphic_allocator<int> pa{ &mbr };
			std::pmr::list<int> list{ pa };
			for (int i{}; i != total_nodes; ++i)
				list.push_back(i);
		};

		const double t1 = benchmark(default_std_alloc, iterations);
		const double t2 = benchmark(default_pmr_alloc, iterations);
		const double t3 = benchmark(pmr_alloc_no_buf, iterations);
		const double t4 = benchmark(pmr_alloc_and_buf, iterations);

		std::cout << std::fixed << std::setprecision(3)
			<< "t1 (default std alloc): " << t1 << " sec; t1/t1: " << t1 / t1 << '\n'
			<< "t2 (default pmr alloc): " << t2 << " sec; t1/t2: " << t1 / t2 << '\n'
			<< "t3 (pmr alloc  no buf): " << t3 << " sec; t1/t3: " << t1 / t3 << '\n'
			<< "t4 (pmr alloc and buf): " << t4 << " sec; t1/t4: " << t1 / t4 << '\n';

		return 0;
	}

	class histogram {
	public:
		histogram(const std::pmr::polymorphic_allocator<>& alloc = {}) : words_(alloc) {}

		void print_top(size_t n, std::ostream& os) const {
			using count = std::pair<std::pmr::string, size_t>;
			std::pmr::vector<count> popular(words_.begin(), words_.end(), words_.get_allocator());
			std::ranges::partial_sort(popular,
				popular.begin() + n,
				std::greater{},
				&count::second);
			for (const auto& stat : popular | std::ranges::views::take(n)) {
				os << stat.first << '\t' << stat.second << '\n';
			}
		}

	private:
		std::pmr::unordered_map<std::string, size_t> words_;
	};



}