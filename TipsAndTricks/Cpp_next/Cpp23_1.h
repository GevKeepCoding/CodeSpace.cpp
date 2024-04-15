#pragma once
#include <mdspan>
#include <experimental/generator>
import std;

using namespace std;

namespace Cpp23_1
{
	// Explicit Object parameter
	// First use case.
	class Person {
	public:
		Person(string name) : m_name{std::move(name)} {}

		/*
		string& getName()& { return m_name; }
		const string& getName() const & { return m_name; }
		string&& getName()&& { return std::move(m_name); }
		*/

		auto&& getName(this auto&& self) {
			return std::forward<decltype(self)>(self).m_name;
		}

	private:
		string m_name;
	};
	// Second use case is recursive lambda
	auto fib = [](this auto self, int n) {
		if (n < 2) return n;
		return self(n - 1) + self(n - 2);
	};

	// if consteval
	// 
	//consteval int f(int i) { return i; }
	//
	//constexpr int g(int i) {
	//	if consteval {
	//		return f(i) + 1;
	//	}
	//	else {
	//		return 42;
	//	}
	//}
	// Multidimentional subscript operator: data[x,y,z]
	//T& operator()(size_t x, size_t y, size_t z) noexcept { /*...*/ }

	void process(int&& value) { std::println("{}", value); }
	void process_all(vector<int>& data) {
		//for (auto i : data) { process(auto(i)); } // auto(i) is a prvalue
	}
	// msvc and icc: __assume(expr)
	// clang: __builtin_assume(expr)
	// gcc: __builtin_assume(expr) or if (expr){} else { __builtin_unreachable(); }

	// [[assume]] for all compilers
	int divide_by_32(int x) {
		//__assume(x >= 0);// before
		//[[assume(x >= 0)]]; // now
		return x / 32;
	}

	// c++ 23 generators
	std::experimental::generator<int> fibs() {
		int a = 0, b = 1;
		while (true) {
			co_yield a;
			b = std::exchange(a, a + b);
		}
	}

	// Performance for std::string
	std::string GeneratePatternOld(const std::string& pattern, size_t count)
	{
		std::string result;
		result.reserve(pattern.size() * count);
		for (size_t i = 0; i < count; ++i)
		{
			result.append(pattern);
		}
		return result;
	}
	std::string GeneratePatternCpp23Faster(const std::string& pattern, size_t count)
	{
		std::string result;
		const auto step = pattern.size();
		result.resize_and_overwrite(step * count, [&](char* buf, size_t n) {
			for (size_t i = 0; i < count; i++) {
				memcpy(buf + i * step, pattern.data(), step);
			}
			return step * count;
		});
		return result;
	}
	void funStackTrace() {
		auto trace{std::stacktrace::current()};
		std::println("{}", trace);
	}

	// print in this format: 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
	void print(std::ranges::input_range auto& r) {
		auto it = r.begin();
		for (const auto& value : r) {
			std::cout << value;
			if (++it != r.end()) {
				std::cout << ", ";
			}
		}
	}

	void println(std::ranges::input_range auto& r) {
		for (auto e : r)
			std::cout << e << ",";

		std::cout << "---\n";
	}
	// Ranges
	void RangesCpp23() {
		{
			// shift_left, shift_right
			vector<string> v{"a", "b", "c", "d", "e"};
			std::ranges::shift_left(v, 2);
			println(v);
		}
		{
			auto ints = std::ranges::views::iota(1, 5) |
				std::ranges::views::transform([](const auto& v) { return v * 2; });
			std::vector vec{std::ranges::to<std::vector>(ints)};
			println(vec);
		}
		{
			std::vector vec{11,22,33};
			std::set set1{std::ranges::to<std::set>(vec)};
			println(set1);
			std::set<double> set2{std::from_range, vec};
			println(set2);
			std::set<double> set3{vec | std::ranges::to<std::set<double>>()};
			println(set3);
		}
		{
			std::string lorem{"lorem ipsum dolor sit amet"};
			auto words{lorem | std::ranges::views::split(' ')
			| std::ranges::views::transform([](const auto& v) {
				return std::string(std::from_range,v); })
				| std::ranges::to<std::vector>()};
			println(words);
			//std::println("{:n:?}", words);
		}
		{
			std::vector v1{1,2,3,4};
			std::vector v2{3,4};
			std::println("{}", std::ranges::contains(v1, 2));
			std::println("{}", std::ranges::contains_subrange(v1, v2));
		}
		{
			std::vector v{1,2,3,4};
			std::println("{}", std::ranges::fold_left(v, 0, std::plus()));
		}
		{
			std::vector v1{1,2};
			std::vector v2{'a', 'b','c'};
			std::vector v3{3,4,5};
			auto r1{std::ranges::views::zip(v1,v2)}; // {(1,'a'), (2,'b')}
			auto r2{std::ranges::views::zip_transform(std::multiplies(),v1,v3)}; // {3,8}
			println(r2);
		}
		{
			std::vector v{1,2,3,4};
			auto r1{v | std::ranges::views::adjacent<2>};// {{1,2},{2,3},{3,4}}
			//auto r3{v | std::ranges::views::adjacent_transform<2>(std::multiplies())};// {{1,2},{2,3},{3,4}}
		}
		{
			std::vector v{1,2,3};
			auto r1{v | std::ranges::views::pairwise}; // {{1,2},{2,3}}
			//auto r2{v | std::ranges::views::pairwise_transform(std::multiplies())}; // {2,6}
		}
		{
			std::vector v{1,2,3,4,5,6,7,8,9,10};
			auto r7{v | std::ranges::views::slide(2)};// {{1,2},{2,3},{3,4}} (same as adjacent)
		}
		{
			std::vector v{1,2,3,4};
			auto r8{v | std::ranges::views::chunk(2)}; // {{1,2},{3}}
		}

		{
			auto print_subrange = [](std::ranges::viewable_range auto&& r)
			{
				std::cout << '[';
				for (int pos{}; auto elem : r)
					std::cout << (pos++ ? " " : "") << elem;
				std::cout << "] ";
			};

			const auto v = {1, 2, 3, 4, 5, 6};

			for (const unsigned width : std::views::iota(1U, 2U + v.size()))
			{
				auto const chunks = v | std::views::chunk(width);
				std::cout << "chunk(" << width << "): ";
				std::ranges::for_each(chunks, print_subrange);
				std::cout << '\n';
			}
		}
		{
			std::vector v{1,2,2,3, 0,4,5,2};
			auto r9{v | std::views::chunk_by(std::ranges::less_equal{})};
			// {(1,2,2,3), (0,4,5), (2)}
		}
		{
			std::vector<std::string> v{"Hello", "World", "!"};
			for (auto c : v | std::views::join_with('\n'))
			{
				std::print("{}", c);
			}
			std::cout << '\n';
		}
		{
			std::vector v{1,2,3,4,5};
			auto r10{v | std::views::stride(2)};
			println(r10);
		}
		{
			auto r11{std::views::repeat(2,3)};// {2,2,2}
			auto r12{std::views::repeat(2)}; // {2,2,2,...}
		}
		{
			std::vector v{0,1,2};
			for (auto&& [a, b] : std::views::cartesian_product(v, v))
			{
				std::println("{} {}", a, b);
			}
		}
		{
			std::vector<std::string> words{"Hello", "World" , "2024"};
			std::vector<std::string> movedWords;
			std::ranges::copy(words | std::views::as_rvalue, std::back_inserter(movedWords));
		}
		{
			std::expected<int, std::string> a{21};
			std::expected<int, std::string> b{std::unexpected("unexpected error"s)};
			if (a.has_value())
				std::println("a has value == {}", a.value());

			if (!b.has_value())
				std::println("b has error {}", b.error());
		}
		{

		}

	}

	// std::move_only_function<>
	int process(std::function<int()> f) {
		return f() * 2;
	}

	int process2(std::move_only_function<int()> f) {
		return f() * 2;
	}

	void Cpp23_Ex2() {
		std::print("{}", process([] { return 21; })); // 42
		//std::print("{}", process([p = std::make_unique<int>(42)] { return *p; })); // compile time error
		std::print("{}", process2([p = std::make_unique<int>(42)] { return *p; })); // compile time error
	}

	void spanStreamEx() {
		char data[] = "11 22";
		std::ispanstream s{std::span<char>{data}};

		int a, b;
		s >> a >> b;
		std::println("a = {}, b = {}", a, b);

		char data2[32]{};
		std::ospanstream s2{std::span<char>{data2}};
		s2 << 22 << 11;
	}
	void swapBytes() {
		{// not tested
			std::uint32_t x = 0x12345678;
			std::uint32_t y = std::endian::native == std::endian::little ?
				std::bit_cast<std::uint32_t>(std::bit_cast<std::array<std::byte, 4>>(x) | std::views::reverse) :
				x;
			std::println("{}", y);
		}
		{
			std::uint32_t a{0x12345678u};
			std::println("{:x}", a);
			std::uint32_t b{std::byteswap(a)};
			std::println("{:x}", b);
		}
	}
	void toUnderlying() {
		cout << "toUnderlying\n";
		enum class E : int { A = 1, B = 2, C = 3 };
		auto e = E::B;
		{// old cpp
			auto i = static_cast<std::underlying_type_t<E>>(e);
			std::println("i == {}", i);
		}
		auto i = std::to_underlying(e);
		std::println("i == {}", i);
	}
}

//
//int main() {
//	RangesCpp23();
//	spanStreamEx();
//	swapBytes();
//	toUnderlying();
//
//
//
//	return 0;
//}

// c++20
void example_latches() {
	vector ppl{"Alex", "Dani", "Benny", "Guy"};

	vector<jthread> workers;
	latch jobs{ptrdiff_t(ppl.size())}, go_home{1};
	auto job = [&jobs, &go_home](string_view name) {
		println("{} is doing a job", name);
		jobs.count_down();
		go_home.wait();
		println("{} is going home", name);
	};

	for (auto& p : ppl) {
		workers.push_back(jthread(job, p));
	}
	jobs.wait();
	println("Go Home!");
	go_home.count_down();
}

void example_barrier() {
	vector ppl{"Alex", "Dani", "Benny", "Guy"};

	vector<jthread> workers;

	auto on_complete = [] {
		static int x = 0;
		not x++ ? println("Go Home!") : println("Done!");
	};
	barrier sync{ptrdiff_t(ppl.size()), on_complete};
	auto job = [&sync](string_view name) {
		println("{} is doing a job", name);
		sync.arrive_and_wait();
		println("{} is going home", name);
		sync.arrive_and_wait();
	};

	for (auto& p : ppl) {
		workers.push_back(jthread(job, p));
	}
}