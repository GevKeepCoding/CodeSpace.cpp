#pragma once
import std;

namespace std_is_trivial_ {

	namespace ex1
	{
		struct S1
		{
			~S1() {} // this is harmful for perf.
		};
		static_assert(!std::is_trivially_destructible_v<S1>);
		static_assert(!std::is_trivially_copyable_v<S1>);
		static_assert(!std::is_trivially_default_constructible_v<S1>);
	}

	namespace ex2
	{
		struct S
		{
			int i;
			S() = default;
			S(int) {}


		};
		static_assert(std::is_trivially_constructible_v<S>);
		static_assert(std::is_trivially_constructible_v<S, const S&>);
		static_assert(!std::is_trivially_constructible_v<S, int>);
		static_assert(std::is_trivial_v<S>);
	}

	namespace ex3 {

		struct S {
			int i{};
		};
		// because default ctor does something:=> it's not default constructible
		static_assert(!std::is_trivially_default_constructible_v<S>);
		static_assert(std::is_trivially_constructible_v<S,const S&>);
		static_assert(std::is_trivially_copy_constructible_v<S>);
		static_assert(!std::is_trivial_v<S>);
	}

	namespace ex4
	{
		struct S {
			int i;
		};
		// below 2 are same
		static_assert(std::is_trivially_constructible_v<S, const S&>);
		static_assert(std::is_trivially_copy_constructible_v<S>);
		//below two are the same thing
		static_assert(std::is_trivially_constructible_v<S, S&&>);
		static_assert(std::is_trivially_move_constructible_v<S>);
	}

	namespace ex5
	{
		struct S
		{
			int i;
			S& operator =(const int a_i) { i = a_i; return *this; }
		};

		static_assert(std::is_trivially_assignable_v<S, const S&>);
		static_assert(!std::is_trivially_assignable_v<S, int>);
	}


	/*
		A trivial class is a class that is trivially copyable and has one or
		more eligible default constructors, all of which are trivial.
		[Note: in particular, a trivially copyable or trivial class does not have virtual
		functions or virtual base classes.-end note]
	
	*/
	namespace ex6
	{
		struct S
		{
			int i;
			virtual void do_stuff() {}
		};
		static_assert(!std::is_trivially_default_constructible_v<S>);
		static_assert(std::is_trivially_destructible_v<S>);
		static_assert(!std::is_trivial_v<S>);

	}

}

int main() {

}



















//Let's make a library that uses Reflection - Konstantinos Kanavouras - Meeting C++ 2023
//https://www.youtube.com/watch?v=YmGIlq7S8WY&list=PLRyNF2Y6sca0KMWJXADniEDufnFdIJz7l&index=3
namespace Magic_Enum_Example
{
	enum class Color
	{
		red,
		green,
		blue
	};

	template<Color C>
	constexpr std::string enum_name()
	{
		// for msvc use __FUNCSIG__
		return std::string(__FUNCSIG__);
		// for gcc use __PRETTY_FUNCTION__ 
	}

	void test()
	{
		std::cout << enum_name<Color::red>() << std::endl;
		std::cout << enum_name<Color::green>() << std::endl;
		std::cout << enum_name<Color::blue>() << std::endl;
	}
	/// similar techniques exist in Boost library
	/*
	enum E2 { a,b,c};

	BOOST_DESCRIBE_ENUM(E2, a,b,c);
	
	struct S { int a; int b; };
	BOOST_DESCRIBE_STRUCT(S, a,b);

	or Boost PFR (only meta-programming)
	struct my_struct { int i; char c; double d; };

	my_struct s{100, 'h', 3.9};
	std::cout << "my_struct has" << boost::pfr::tuple_size<my_struct>::value << "fields\n";
	std::cout << "fields: " << boost::pfr::io(s) << "\n";
	*/


	/*
		C++ Runtime reflection in MSVC compiler:

		using namespace System;
		enum class Options { Option1, Option2 };

		int maint () {
			array<String^>^ names = Enum::GetNames(Options::typeid);
			Console::WriteLine("There are {0} options in enum '{1}'", names->Length, Options::typeid->Name);
			for (int i =0; i < names->Length; i++)
				Console::WriteLine("{0}: {1}", i, names[i]);

			Options o = Options::Option2;
			Console::WriteLine("value of 'o' is {0}",o);
		}
	
	
	*/

	/*
		Circle compiler

		template<typename type_t>
		const char* name_from_enum(type_t x) {
			static_assert(std::is_enum<type_t>::value);

			switch(x) {
				@meta for (int i = 0; i < @enum_count(type_t); ++i) {
					case @enum_value(type_t, i);
					return @enum_name(type_t, i);
				}
			default:
			return nullptr;
			}
		}
	*/



}



namespace CppKnowledge
{
	void fun1()
	{
		struct A
		{
			std::string name;
			std::size_t size;
			A() = default;
		};
		{
			A a;
			std::cout << "name: " << a.name << " size: " << a.size << std::endl;
			/*
				above code a.size is UB, because there is not ctor call.
			*/
		}
		{
			A a{};
			std::cout << "name: " << a.name << " size: " << a.size << std::endl;
			/*
				above code a.size is 0, because of a{}
			*/
		}
	}

	void NoUB()
	{
		struct A
		{
			std::string name{};
			std::size_t size{};// this {} won't allow ub anymore.
		};
		{
			A a;
			std::cout << "name: " << a.name << " size: " << a.size << std::endl;
			/*
				a.size is not UB
			*/
		}
		{
			A a{};
			std::cout << "name: " << a.name << " size: " << a.size << std::endl;
			/*
				a.size is not UB
			*/
		}
	}


	//-----------------------

	void Packing_Struct_To_Reduce_Memory_Use()
	{

#pragma pack(push,1)
		struct A {
			int x;
			short y;
		};
#pragma pack(pop)

		static_assert(sizeof(A) == 6);
	}


	//-----------------------

	void Default_Ctor_not_always_called()
	{
		struct Point
		{
			int x, y;

			Point(int x1, int y1) : x(x1), y(y1)
			{
			}
			Point() = default;
			/*Point() {
				std::println("default constructor {} {}", x, y);
			}*/
			//Point() = delete;// default;
			void print() const { std::cout << x << " " << y << std::endl; }
		};

		class Rectangle
		{
			Point p1, p2;
		public:
			Rectangle(Point p1, Point p2) {// : p1(p1), p2(p2) {
				this->p1.print();
				this->p2.print();
			}
		};
		// in msvc debug x64, it will not initialize x,y to 0,0,
		// but in release x64, it will.


		Point p({1}, {2});
		Rectangle r(p, p);
		//Rectangle r1(Point{1, 2}, Point{3, 4});

	}



}







using namespace CppKnowledge;

//int main()
//{
//	fun1();
//	return 0;
//}