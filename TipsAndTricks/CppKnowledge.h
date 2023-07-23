#pragma once
import std;


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