#pragma once
import std;
using namespace std;

namespace p2
{

class spin_lock
{
public:
	void lock() noexcept {

	}
	void unlock() noexcept {

	}
	bool try_lock() noexcept {

	}
private:
	std::atomic_flag flag{};
};

// make x = x * y, but atomically
void atomic_multiply(std::atomic<int>& x, int y)
{

}



}