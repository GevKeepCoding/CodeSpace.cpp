#pragma once
import std;

//Symbolic Calculus for High-performance Computing From Scratch Using C++23 - Vincent Reverdy - CppCon
//https://www.youtube.com/watch?v=lPfA4SFojao&t=3s
namespace Symbolic_Calculus1
{
	template<class = decltype([] {}) >
	struct symbol {};
	// we can do the same with auto
	template<auto = [] {} > // this is called NTTP
	struct symbol2 {};
}

namespace Symbolic_Calculus2
{
	template<class>
	struct symbol_id {};

	template<auto Id = symbol_id<decltype([] {}) > {} >
	struct symbol {
		static constexpr auto id = Id;
	};
}

namespace Symbolic_Calculus3 {
	template<class>
	struct symbol_id {
		static constexpr auto singleton = [] {};
		static constexpr const void* address = std::addressof(singleton);
	};

	template<auto Id = symbol_id<decltype([] {}) > {} >
	struct symbol {
		static constexpr auto id = Id;
	}; // now every symbol will have unique address and id

	//template<class Lhs, class Rhs>
	//constexpr bool operator <(symbol_id<Lhs>, symbol_id<Rhs>) {
	//	return symbol_id<Lhs>::address < symbol_id<Rhs>::address;
	//}//Cpp standard says that comparing pointers that don't point to the same array is undefined

	//template<class Lhs, class Rhs>
	//constexpr bool operator <(symbol_id<Lhs>, symbol_id<Rhs>) {
	//	return std::less{}(symbol_id<Lhs>::address, symbol_id<Rhs>::address);
	//}// std::less can safely compare two pointers.

	// There is even better way

	template<class Lhs, class Rhs>
	constexpr std::strong_ordering operator <=>(symbol_id<Lhs>, symbol_id<Rhs>) {
		return std::compare_three_way{}(
			symbol_id<Lhs>::address,
			symbol_id<Rhs>::address);
	}



	template<class Symbol, class T>
	struct symbol_binder {
		constexpr symbol_binder(Symbol, T x) : value(x) {}
		static constexpr Symbol symbol = {};
		T value;
	};

}


namespace Symbolic_Calculus4 {
	template<class>
	struct symbol_id {
		static constexpr auto singleton = [] {};
		static constexpr const void* address = std::addressof(singleton);
	};


	template<class T>
	struct remove_rvalue_reference : std::type_identity<T> {};

	template<class T>
		requires std::is_rvalue_reference_v<T>
	struct remove_rvalue_reference<T> : std::type_identity<std::remove_reference_t<T>> {};

	template<class T>
	using remove_rvalue_reference_t = remove_rvalue_reference<T>::type;


	template<class T>
	struct requalify_as_const : std::conditional<
		std::is_lvalue_reference_v<T>,
		std::add_lvalue_reference_t<std::add_const_t<std::remove_reference_t<T>>>,
		std::conditional_t<
		std::is_rvalue_reference_v<T>,
		std::add_rvalue_reference_t<std::add_const_t<std::remove_reference_t<T>>>,
		std::add_const_t<T>>> {};

	template<class T>
	using requalify_as_const_t = typename requalify_as_const<T>::type;


	template<class Symbol, class T>
	struct symbol_binder {
		using symbol_type = Symbol;
		using value_type = std::remove_cvref_t<T>;
		static constexpr Symbol symbol = {};

		// Constructors
		template<class U>
			requires std::is_convertible_v<U&&, requalify_as_const_t<remove_rvalue_reference_t<T>>>
		constexpr symbol_binder(Symbol, U&& x) noexcept(
			std::is_nothrow_convertible_v<U&&, requalify_as_const_t<remove_rvalue_reference_t<T>>>
			)
			: value(std::forward<U>(x)) {}

		// Accessors
		const value_type& operator()() const noexcept { return value; }
	private:
		requalify_as_const_t<remove_rvalue_reference_t<T>> value;
	};
	// Deduction guide
	template<class Symbol, class T>
	symbol_binder(Symbol, T&&) -> symbol_binder<Symbol, T&&>;

	// Symbol
	template<auto Id = symbol_id<decltype([] {}) > {} >
	struct symbol1 {
		static constexpr auto id = Id;
		template<class T>
		constexpr symbol_binder<symbol1, T&&> operator=(T&& value) {
			return symbol_binder(*this, std::forward<T>(value));
		}
	};
	namespace a1 {

		struct unconstrained {
			template<class T>
			constexpr std::true_type operator()(T value) const noexcept { return {}; }
		};

		struct real {
			template<class T>
			constexpr std::false_type operator()(T value) const noexcept { return {}; }

			template<class T>
				requires std::is_floating_point_v<T>
			constexpr std::true_type operator()(T x) const noexcept { return {}; }
		};



		template<class T = unconstrained, auto Id = symbol_id<decltype([] {}) > {} >
		struct symbol {
			static constexpr auto id = Id;
			template<class Arg>
				requires decltype(std::declval<T>()(std::declval<Arg>()))::value
			constexpr symbol_binder<symbol, Arg&&> operator=(Arg&& value) {
				return symbol_binder(*this, std::forward<Arg>(value));
			}
		};
	}
	namespace a2 {

		struct unconstrained {
			template<class T>
			struct trait : std::true_type {};
		};

		struct real {
			template<class T>
			struct trait : std::is_floating_point<T> {};
		};



		template<class T = unconstrained, auto Id = symbol_id<decltype([] {}) > {} >
		struct symbol {
			static constexpr auto id = Id;
			template<class Arg>
				requires T::typename trait<std::remove_cvref_t<Arg>>::value
			constexpr symbol_binder<symbol, Arg&&> operator=(Arg&& value) {
				return symbol_binder(*this, std::forward<Arg>(value));
			}
		};
	}

	template<class Lhs, class Rhs>
	constexpr std::strong_ordering operator <=>(symbol_id<Lhs>, symbol_id<Rhs>) {
		return std::compare_three_way{}(
			symbol_id<Lhs>::address,
			symbol_id<Rhs>::address);
	}

	// variable symbols: x, y, z, ...
	// constant symbols: pi, e, ...
	// function symbols: sin, cos, exp, log, ...
	//	* operator symbols: +, -, *

	// Free and bound variables
	// free variables: variables that act as symbolic placeholders.
	// bound variables: variables that have been bound to a particular value

}


int main() {

}



