#pragma once
#include <array>
#include <vector>
#include <memory_resource>


namespace {
	void process() {
		
		std::array<float, 1024> stack_memory;
		std::pmr::monotonic_buffer_resource monotonic_buffer(
			stack_memory.data(), stack_memory.size(),
			std::pmr::null_memory_resource()
		);
		
		using allocator_t = std::pmr::polymorphic_allocator<float>;
		allocator_t allocator(&monotonic_buffer);

		std::pmr::vector<float> myvector(1024, 0.0f, allocator);
		//...
	}
}