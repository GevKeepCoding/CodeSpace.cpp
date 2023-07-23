#include <tuple>
import std;

// since C++23
//struct Slice : public std::ranges::range_adaptor_closure<Slice>
//{
//	std::size_t start = 0;
//	std::size_t end = std::string_view::npos;
//
//	std::string_view operator()(std::string_view sv) const {
//		return sv.substr(start, end - start);
//	}
//};
//
//void Example1() {
//	std::string str = "01234567";
//
//	Slice slicer{.start = 1u, .end = 6u};
//
//	auto sv1 = slicer(str);
//	auto sv2 = str | slicer;
//
//	std::cout << sv1 << '\n';
//	std::cout << sv2 << '\n';
//
//	auto slice_and_drop = slicer |
//		std::ranges::views::drop_while([](auto ch) { return ch != '3'; });
//
//	std::cout << std::string_view(str | slice_and_drop) << '\n';
//}


//struct Slice2;

//struct Package {
//	Slice2* obj;
//	std::tuple<int, int> args;
//};

//template<class Derived>
//struct Piper {
//
//	Package make_package(int start, int end) {
//		return Package{static_cast<Derived*>(this), {start, end}};
//	}
//
//};

//
//struct Slice2 : Piper<Slice2>
//{
//	auto operator()(std::string_view sv, int start, int end) const {
//		return sv.substr(start, end - start);
//	}
//	auto operator()(int start, int end) {
//		return static_cast<Piper<Slice2>*>(this)->make_package(start, end);
//	}
//
//};// "hello world" | slicer(0,5);
//
//auto operator|(std::string_view sv, Package p) {
//	return p.obj->operator()(sv, std::get<0>(p.args), std::get<1>(p.args));
//}

//https://www.youtube.com/watch?v=5XYWyPjxRuE
//2013 12 - Ali Almutawa Jr - range_adapter_closure - But Better!

namespace detail2 
{
	template<typename Derived>
		//requires std::is_object_v<Derived>&& std::same_as<Derived, std::remove_cvref_t<Derived>>
	class Piper;

	template<class T>
	concept PiperType =
		std::is_base_of_v<Piper<std::remove_cvref_t<T>>, std::remove_cvref_t<T>>;

	template<PiperType T, typename...Args>
	struct Package {
		const T* derived;
		std::tuple<Args...> args;

		template<typename V>
		inline constexpr auto operator()(V&& v) const {
			return operator|(std::forward<V>(v), *this);
		}
		inline constexpr Package& operator()() {
			return *this;
		}

	};
	// add deduction guide for Package
	template<typename T, typename...Args>
	Package(T,Args...) -> Package<T,Args...>;

	template<typename Derived>
	//requires std::is_object_v<Derived>&& std::same_as<Derived, std::remove_cvref_t<Derived>>
	class Piper {
	public:
		template<class...Args>
		auto make_package(Args... args) const {
			return Package<Derived, Args...>{static_cast<const Derived*>(this), {args...}};
		}//<std::remove_cvref_t<Derived>,std::remove_cvref_t<Args>...>
	};


	template<typename T>
	concept Packaged = requires (T t) {
		t.derived; t.args;
	};

	
	void UseCase() {
		std::vector v{1,2,3,4,5};
		// v | filter(isEven) | map(doubleIt) | fold(0, std::plus{});
		// auto mapFold = map(doubleIt) | fold(0, std::plus{});
		// auto filterMapFold = filter(isEven) | mapFold;
		// v | filterMapFold;

	}
	template<Packaged F1, Packaged F2>
	struct Composed : Piper<Composed<F1, F2>> {
		F1 f1;
		F2 f2;
		Composed(F1 f1, F2 f2) : f1(f1), f2(f2) {}

		auto operator()(auto t) const { return f2(f1(t)); }

		template<typename... Types>
		constexpr auto operator()(Types&&... params) const {
			return f2(f1(std::forward<Types>(params)...));
		}

		[[nodiscard]] constexpr auto operator()() const {
			return static_cast<const Piper*>(this)->make_package();
		}

	};







	struct Fold : Piper<Fold> {
		template<std::ranges::range V, typename INIT = int, std::invocable<INIT, std::ranges::range_value_t<V>> OP = decltype(std::plus{}) >
		constexpr INIT operator()(V&& v, INIT init = {}, OP op = std::plus{}) const {
			for (auto&& e : v) {
				init = op(init, e);
			}
			return init;
		};
		template<typename INIT = int, typename OP = decltype(std::plus{}) >
		constexpr auto operator()(INIT init = {}, OP op = std::plus{}) const {
			return static_cast<const Piper*>(this)->make_package(init, op);
		}
	};

	struct Map : Piper<Map> {
		template<std::ranges::range V, typename INIT = int, std::invocable<INIT, std::ranges::range_value_t<V>> OP >
		constexpr INIT operator()(V&& v, OP op) const {
			std::remove_cvref_t<V> result;
			std::back_insert_iterator<std::remove_cvref_t<V>> it(result);

			for (auto&& e : v) {
				*it = op(e);
			}

			return result;
		};

		constexpr auto operator()(auto func) const {
			return static_cast<const Piper*>(this)->make_package(func);
		}
	};
	Map mapper;

	struct Filter : Piper<Filter>
	{
		template<std::ranges::range V, std::invocable<std::ranges::range_value_t<V>> Pred>
		constexpr auto operator()(V v, Pred pred) const {
			V v2;
			auto insert = std::back_inserter(v2);

			for (auto&& e : v) {
				if (pred(e)) {
					insert = e;
				}
			}
			return v2;
		}

		constexpr auto operator()(auto pred) const {
			return static_cast<const Piper*>(this)->make_package(pred);
		}
	};


	struct Slice : Piper<Slice>
	{
		auto operator()(std::string_view sv, int start, int end) const {
			return sv.substr(start, end - start);
		}
		auto operator()(int start, int end) const {
			return static_cast<const Piper*>(this)->make_package(start, end);
		}

	};// "hello world" | slicer(0,5);
	Slice slicer;
	//auto operator|(std::string_view sv, Package p) {
	//	return p.obj->operator()(sv, std::get<0>(p.args), std::get<1>(p.args));
	//}

	template<typename F1, typename F2>
		requires ((PiperType<F1> || Packaged<F1>) && (PiperType<F2> || Packaged<F2>))
	inline constexpr auto operator |(F1&& f1, F2&& f2) {
		if constexpr (PiperType<F1> && PiperType<F2>)
		{
			using P1 = decltype(f1());
			using P2 = decltype(f2());
			return Composed<P1, P2>{f1(), f2()};
		}
		else if constexpr (PiperType<F1>) {
			using P1 = decltype(f1());
			return Composed<P1, F2>(f1(), f2);
		}
		else if constexpr (PiperType<F2>) {
			using P2 = decltype(f2());
			return Composed<F1, P2>(f1, f2());
		}
		else {
			return Composed<F1, F2>(f1, f2);
		}
	}

	template<size_t...Is>
	auto piperHelper(std::index_sequence<Is...>, Packaged auto p, auto v) {
		return p.derived->operator()(v, std::get<Is>(p.args)...);
	}


	template<typename V, Packaged P>
	requires (!Packaged<V> && !PiperType<V>)
	constexpr auto operator|(V&& v, P&& p) {
		auto size = std::tuple_size<decltype(p.args)>{};
		return piperHelper(std::make_index_sequence<size>{}, std::forward<P>(p),
						   std::forward<V>(v));
	}
	
	template<typename V>
		requires (!Packaged<V> && !PiperType<V>)
	constexpr auto operator|(V&& v, PiperType auto&& p) {
		return operator|(std::forward<V>(v), p());
	}
}


detail2::Filter filter;
detail2::Fold folder;

using namespace detail2;

int main() {

	std::string name = "Ali Almutawa";
	auto first3 = slicer(0, 3);
	auto capitalize = mapper([](auto&& s) { return std::toupper(s); });

	auto capFirst3 = capitalize | first3;

	//std::cout << (name | capFirst3()) << "\n";???


	/*auto a = "hello world" | Slice2{}(0, 5);
	for (auto e : a) {
		std::cout << e;
	}*/
	return 0;
}



