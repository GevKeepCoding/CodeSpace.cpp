#pragma once
import std;




template<class T, std::size_t Size, std::size_t Alignment>
class FastPimpl final {
public:

	~FastPimpl() noexcept {// called only from .cpp file not from .h file
		Validate<sizeof(T), alignof(T)>();
		reinterpret_cast<T*>(&storage)->~T();
	}

private:
	template<std::size_t Size2, std::size_t Alignment2>
	void Validate() const noexcept {
		static_assert(Size == Size2, "Size mismatch");
		static_assert(Alignment == Alignment2, "Alignment mismatch");
	}
	// cpp26 
	/*
	template<std::size_t Size2, std::size_t Alignment2>
	void Validate() const noexcept {
		static_assert(Size == Size2, 
		fmt::format("Template argument 'Size' should be {}", Size2).c_str());
		static_assert(Alignment == Alignment2, "Alignment mismatch");
	}
	
	*/


	alignas(Alignment) std::byte storage[Size];
};
