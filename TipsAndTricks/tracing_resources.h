#pragma once
#include <ostream>



class tracing_resource01 final : public std::pmr::memory_resource
{
public:
	tracing_resource01(std::ostream& os) : os_(os) {}
private:
	void* do_allocate(size_t bytes, size_t align) override
	{
		auto addr = operator new(bytes, std::align_val_t(align));
		os_ << "allocate(" << bytes << ", " << align << ") = " << addr << "\n";
		return addr;
	}
	void do_deallocate(void* addr, size_t bytes, size_t alignment) override
	{
		operator delete(addr, bytes, std::align_val_t(alignment));
		os_ << "deallocate(" << addr << ", " << bytes << ", " << alignment << ")\n";
	}

	std::ostream& os_;
};

class tracing_resource final : public std::pmr::memory_resource
{
public:
	tracing_resource(std::ostream& os,
		std::pmr::memory_resource* next = std::pmr::get_default_resource()) : os_(os), next_(next)
	{}
private:
	void* do_allocate(size_t bytes, size_t align) override {
		auto addr = next_->allocate(bytes, align);
		os_ << "allocate(" << bytes << ", " << align << ") = " << addr << "\n";
		return addr;
	}
	void do_deallocate(void* addr, size_t bytes, size_t alignment) override {
		os_ << "deallocate(" << addr << ", " << bytes << ", " << alignment << ")\n";
		next_->deallocate(addr, bytes, alignment);
	}
	bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
		auto p = dynamic_cast<const tracing_resource*>(&other);
		return p && next_ == p->next_;
	}

	std::ostream& os_;
	std::pmr::memory_resource* next_;
};
