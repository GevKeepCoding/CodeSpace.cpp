import std;


int g_x = 0;

__declspec(noinline)
void h(const int& r) {
	std::cout << std::addressof(r) << "   " << &r << std::endl;
}

void g(void* ptr) {

	h(*reinterpret_cast<int*>(ptr));
}

__declspec(noinline)
void g1(void* ptr) {
	alignas(int) std::byte buffer[sizeof(int)];
	const auto retr = std::memcpy(buffer, ptr, sizeof(int));
	h(*reinterpret_cast<int*>(retr));
}

void f() {
	std::cout << std::addressof(g_x) << "   " << &g_x << std::endl;
	const void* ptr = &g_x;
	g1(&g_x);
}


int main()
{
	f();
	f();
	f();
	f();
	f();
	f();
	f();
	f();
	f();
	f();


	return 0;
}