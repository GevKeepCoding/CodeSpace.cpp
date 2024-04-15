#pragma once
#include <atomic>
#include <iostream>
import std;
using namespace std;


// simple spin lock class that uses atomic_flag
class spin_lock
{
public:
	void lock() noexcept { while (flag.test_and_set()); }
	void unlock() noexcept { flag.clear(); }
	bool try_lock() noexcept { return !flag.test_and_set(); }

private:
	std::atomic_flag flag{};
};

// let's do "atomic" multiplication.
void atomic_multiply(std::atomic<int>& x, int y) {
	int x0 = x;
	while (!x.compare_exchange_strong(x0, y * x0)) {}
}






int main() {
	spin_lock s{};

	s.lock();
	if (s.try_lock()) {
		cout << "lock acquired" << endl;
	}
	else {
		cout << "lock not acquired" << endl;
	}
}












