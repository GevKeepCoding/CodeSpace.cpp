#pragma once
namespace cpp26
{
	/*
	1. 
	auto s = std::to_string(1e-7); // C++20: "0.00000" or "0,00000" depending on locale
	in c++26 to_string will return most short representation of floating point arg
	in this case it may be: "1e-7" 
	
	2. Hazart Pointer
	struct Data : std::hazard_pointer_obj_base<Data>
	{};
	
	std::atomic<Data*> pdata;

	template<typename Func>
	void reader_op(Func userFn) {
		std::hazard_pointer h = std::make_hazard_pointer();
		Data* p = h.protect(pdata);
		userFn(p);
	}
	
	void Writer(Data* newData) {
		Data* old = pdata.exchange(newData);
		old->retire();
	}

	//				RCU 
	struct Data : std::rcu_obj_base<Data>
	{};

	std::atomic<Data*> pdata;
	template<typename Func>
	void reader_op(Func userFn) {
		std::scoped_lock l(std::rcu_default_domain());
		Data* p = pdata;
		userFn(p);
	}

	void Writer(Data* newData) {
		Data* old = pdata.exchange(newData);
		if (old) old->retire();
	}

	void shutdown() {
		writer(nullptr);
		std::rcu_synchronize(); // wait until all previous readers won't unlock their locks.
		std::rcu_barrier();// delete the remaining.
	}

	// second way to use RCU ( when we want to delete objects right away)
	struct Data{};
	std::atomic<Data*> pdata;
	
	// reader is the same

	void Writer(Data* newData) {
	    Data* old = pdata.exchange(newData);
		
		std::rcu_synchronize(); // wait until it's safe
		delete old;
	}

	void shutdown() {
		writer(nullptr);
	}


	------------------------
		std::move_only_function<void(void)> f; //C++23
	f = [p = std::make_unique<int>()]() {
		std::cout << p.get();
	};

	f();

	// C++26
	std::function_ref<void(void)> f1;
	f1 = []() { std::cout << "done"; };
	f1();

	std::copyable_function is repaired std::function

	----------------------



	*/
}