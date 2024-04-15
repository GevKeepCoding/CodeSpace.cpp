#pragma once
#include <atomic>
#include <benchmark/benchmark.h>
#include "../Helper/simplify.h"
#include <mutex>

#include <boost/smart_ptr/detail/spinlock.hpp>
#include <boost/smart_ptr/detail/spinlock_pool.hpp>
#include <iostream>


// Concurrency in C++: A Programmer’s Overview(part 2 of 2) - Fedor Pikus - CppNow 2022 : https://www.youtube.com/watch?v=R0V4xJ9HZpA
namespace a1
{

std::atomic<unsigned long>* p(new std::atomic<unsigned long>);
unsigned long* q(new unsigned long);

void bm_write_nonatomic(benchmark::State& state) {
	if (state.thread_index() == 0) *q = 0;

	for (auto _ : state) {
		REPEAT(
			benchmark::DoNotOptimize(*q = 1);
			);
	}

	state.SetItemsProcessed(64 * state.iterations());
}

void bm_write_atomic(benchmark::State& state) {
	if (state.thread_index() == 0) p->store(0);

	for (auto _ : state) {
		REPEAT(
			p->store(42, std::memory_order_relaxed);
			);
	}

	state.SetItemsProcessed(64 * state.iterations());
}

void bm_increment_mutex(benchmark::State& state) {
	if (state.thread_index() == 0) *q = 0;
	static std::mutex m;
	for (auto _ : state) {
		REPEAT(
			{
				std::lock_guard<std::mutex> lock(m);
				*q += 1;
			}
		);
	}

	state.SetItemsProcessed(64 * state.iterations());
}
void bm_atomic_increment(benchmark::State& state) {
	if (state.thread_index() == 0) p->store(0);

	for (auto _ : state) {
		REPEAT(
			++*p;
			);
	}

	state.SetItemsProcessed(64 * state.iterations());
}
void bm_fetch_add(benchmark::State& state) {
	if (state.thread_index() == 0) p->store(0);

	for (auto _ : state) {
		REPEAT(
			p->fetch_add(1, std::memory_order_relaxed);
			);
	}

	state.SetItemsProcessed(64 * state.iterations());
}
void bm_increment_cas(benchmark::State& state) {
	if (state.thread_index() == 0) *p = 0;

	for (auto _ : state) {
		REPEAT(
			auto xl = p->load(std::memory_order_relaxed);
		while (!p->compare_exchange_strong(xl, xl + 1)) {}
			);
	}
	state.SetItemsProcessed(64 * state.iterations());
}
void bm_increment_cas_weak(benchmark::State& state) {
	if (state.thread_index() == 0) *p = 0;

	for (auto _ : state) {
		REPEAT(auto xl = p->load(std::memory_order_relaxed);
		while (!p->compare_exchange_weak(xl, xl + 1)) {}
			);
	}
	state.SetItemsProcessed(64 * state.iterations());
}

// using good spin_lock we can get better performance than using above techniques.
// OK, maybe depending on a number of threads.
// we can try to use spin lock from boost
void bm_spinlock(benchmark::State& state) {
	if (state.thread_index() == 0) *p = 0;
	boost::detail::spinlock m;
	for (auto _ : state) {
		REPEAT(
			boost::detail::spinlock::scoped_lock lock(m);
		++*p;
			);
	}

	state.SetItemsProcessed(64 * state.iterations());
}

const int max_threads = std::thread::hardware_concurrency();

#define ARGS \
	->ThreadRange(4,4)->UseRealTime()


//BENCHMARK(bm_write_nonatomic) ARGS;
//BENCHMARK(bm_write_atomic) ARGS;



//BENCHMARK(bm_increment_mutex) ARGS;
//BENCHMARK(bm_atomic_increment) ARGS;
//BENCHMARK(bm_fetch_add) ARGS;
//BENCHMARK(bm_increment_cas) ARGS;
//BENCHMARK(bm_increment_cas_weak) ARGS;
//BENCHMARK(bm_spinlock) ARGS;




void test_is_atomic_lock_free() {

	// Only std::atomic_flag is guaranteed to be lock-free.
	//constexpr std::atomic<T>::is_always_lock_free

	long x;
	struct A { long x; };
	struct B { long x; long y; };
	struct C { long x; int y; };
	struct D { int x; int y; int z; };
	struct E { long x; long y; long z; };

	std::cout << "atomic<long> is lock free: " << std::atomic<long>().is_lock_free() << std::endl;
	std::cout << "atomic<A> is lock free: " << std::atomic<A>().is_lock_free() << std::endl;
	std::cout << "atomic<B> is lock free: " << std::atomic<B>().is_lock_free() << std::endl;
	std::cout << "atomic<C> is lock free: " << std::atomic<C>().is_lock_free() << std::endl;
}

std::atomic<long> a[1024] = {};
void bm_shared(benchmark::State& state)
{
	for (auto _ : state) {
		benchmark::DoNotOptimize(a[0].fetch_add(1));
	}
}
void bm_false_shared(benchmark::State& state)
{
	const int i = state.thread_index();
	for (auto _ : state) {
		benchmark::DoNotOptimize(a[i].fetch_add(1));
	}
}
void bm_not_shared(benchmark::State& state)
{
	const int i = state.thread_index() * std::hardware_destructive_interference_size / sizeof(a[0]);
	for (auto _ : state) {
		benchmark::DoNotOptimize(a[i].fetch_add(1));
	}
}

#define ARGS \
	->ThreadRange(1, max_threads)->UseRealTime()

//BENCHMARK(bm_shared) ARGS;
//BENCHMARK(bm_false_shared) ARGS;
//BENCHMARK(bm_not_shared) ARGS;



void example1()
{
	struct node { int value; node* next; };
	std::atomic<node*> head;

	void push_front(int x) {
		node* new_node = new node;
		new_node->value = x;
		node* old_head = head.load();
		do {
			new_node->next = old_head;
		} while (!head.compare_exchange_strong(old_head, new_node));
	}
}

void example2()
{
	int x = 0;
	// Lock l;
	// l.lock();
	//++x;
	//l.unlock();
	std::atomic<int> l(0);
	
	while(l.exchange(1, std::memory_order_acquire));
	
	++x;
	
	l.store(0, std::memory_order_release);


}

void example3() {
	int a = 0;
	std::atomic<int> ready{0};
	//Thread 1
	a = 42;
	std::atomic_thread_fence(std::memory_order_release);
	ready.store(1, std::memory_order_relaxed);
	// Thread 2
	int r1 = ready.load(std::memory_order_relaxed);
	std::atomic_thread_fence(std::memory_order_acquire);

	int r2 = a;

	//--------------------------------------------
	//	Better approach
	//Thread 1
	a = 42;
	ready.store(1, std::memory_order_release);
	// Thread 2
	int r1 = ready.load(std::memory_order_acquire);

	int r2 = a;

}






}


