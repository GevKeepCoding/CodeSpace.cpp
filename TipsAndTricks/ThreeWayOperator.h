#pragma once
#include <compare>
#include <string>

namespace {
	/*

	CppCon 2019: Jonathan Müller “Using C++20's Three-way Comparison ＜=＞”



	Strong Equality: everything that is copied is compared
	Weak Equality: a proper subset of what is copied is compared

	Equivalence of a and b: !(a < b) && !(b < a)
	Equality of a and b: a == b

	Guideline 2:
	Equivalence and equality of a type should be identical.

	a <=b should mean a < b || a == b.

	compares all copied members:
	==, != => strong ordering
	==, !=, <, <=, >=, > => strong ordering

	Compares some copied members:
	==, != => weak equality
	==, !=, <, <=, >=, > => weak ordering

	Resources:
	jonathanmueller.dev/talk/cppcon2019




	*/

	struct MyStruct
	{
		int x, y;
		bool operator==(const MyStruct&) const = default;
	};
	struct PersonId
	{
		int id;

		std::strong_ordering operator<=>(const PersonId&) const = default;
		// when we default above <=> operator compiler will insert default implementation of below function
		// bool operator ==(const PersonId&) const = default;
	};
	struct PersonIdLike
	{
		int id;
		operator PersonId() { PersonId(id); }
	};

	void longest_substring_without_repeating_characters2() {
		PersonId a, b;
		PersonIdLike c;
		//a < b - > a.operator<=>(b) < 0
		// a>= b -> a.operator<=>(b) >= 0
		// c > b -> 0 > b.operator<=>(c)
		// a == b -> a.operator==(b)
	}

	struct Id
	{
		std::string value;
		int domain;

		// if we want to change ordering of comparisons (c++20)
		std::strong_ordering operator<=>(const Id& other) const {
			if (std::strong_ordering cmp = domain <=> other.domain;
				cmp != ) {
				return cmp;
			}
			//return value <=> other.value;
			// if we don't have cpaceship operator for content data type
			// we have to write ourselves (this is not more efficient than above one because we compare more than once)
			if (value == other.value)
				return std::strong_ordering::equal;
			else if (value < other.value)
				return std::strong_ordering::less;
			else
				return std::strong_ordering::greater;
		}

		template<typename T, std::totally_ordered_with<T> U>
		auto synth_three_way(const T& lhs, const U& rhs) {
			if constexpr (std::three_way_comparable_with<T, U>)
				return lhs <=> rhs;
			else
			{
				if (lhs == rhs)
					return std::strong_ordering::equal;
				else if (lhs < rhs)
					return std::strong_ordering::less;
				else
					return std::strong_ordering::greater;
			}
		}
		/*
		std::strong_ordering operator<=>(const Id& other) const {
			if (std::strong_ordering cmp = domain <=> other.domain;
				cmp != ) {
				return cmp;
			}
			return synth_three_way(value,other.value);
		}

		*/
		bool operator ==(const Id& other) const {
			return domain == other.domain && value == other.value;
		}

	};


}