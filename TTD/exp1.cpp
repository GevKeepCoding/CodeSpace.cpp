#include <iostream>
#include <array>


/*
dx @$curprocess
then click TTD node
then click events


## [Pacific++ 2018: James McNellis "Time Travel Debugging"](https://www.youtube.com/watch?v=BVslyei0804)



*/


void Fun(wchar_t* buffer, size_t size) {
	wchar_t const* const message = L"HELLO TO EVERYONE, THIS COPY WILL CURRAPT EVERYTHING AFTER IT........";
	wcscat_s(buffer, size, message);
}


int main()
{
	int* i = new int;
	std::cout << *i;
	std::array<wchar_t, 50> greeting{};
	Fun(greeting.data(), sizeof(greeting));

	wprintf(L"%ls\n", greeting.data());
}