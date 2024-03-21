#pragma once
import std;




namespace reconstruct
{
	void corge(const int&) noexcept {};// body may be often hidden for the compiler.
	void quux(void* ptr) noexcept {
		corge(*reinterpret_cast<int*>(ptr));
	}
	void quux_well_defined_bad_codegen(const void* ptr) noexcept { // this is because corge could have compared address of it's parameter to some global
		// therefore cpp compiler is not allowed to do certain optimizations.
		alignas(int) std::byte buffer[sizeof(int)];
		const auto result = std::memcpy(buffer, ptr, sizeof(int));
		corge(*reinterpret_cast<int*>(result));
	}
	void quux_well_defined_fast(const void* ptr) noexcept {
		const auto mutable_ptr = const_cast<void*>(ptr);
		const auto byte_ptr = new (mutable_ptr) std::byte[sizeof(int)];
		const auto int_ptr = reinterpret_cast<const int*>(byte_ptr);
		corge(*int_ptr);
	}

	// the above code has a good codegen(same as for quux) and is well defined
	// but we can do better and not write the boilerplate code above.
	// cpp23 has start_lifetime_as and start_lifetime_as_array functions for this.

	/*void quux_Cpp23_well_defined_fast(const void* ptr) noexcept {
		corge(*std::start_lifetime_as<int>(ptr));
	}*/

	template<typename T>
	const T* start_lifetime_as(const void* p) noexcept {
		const auto mp = const_cast<void*>(p);
		const auto bytes = new(mp) std::byte[sizeof(T)];
		const auto ptr = reinterpret_cast<const T*>(bytes);
		(void)*ptr;
		return ptr;
	}
	template<class T>
	const T* start_lifetime_as_array(const void* p, std::size_t n) noexcept {
		const auto mp = const_cast<void*>(p);
		const auto bytes = new(mp) std::byte[n * sizeof(T)];
		const auto ptr = reinterpret_cast<const T*>(bytes);
		(void)*ptr;
		return ptr;
	}


#pragma pack(push)
#pragma pack(4) // make class objects 4 byte aligned to save space
	struct erased_update
	{
		std::uint64_t raw_timestamp;
		std::uint32_t length;
		std::uint64_t timestamp() const noexcept {
			alignas(std::uint64_t) std::byte buffer[sizeof(std::uint64_t)];
			const auto ptr = std::memcpy(buffer, &raw_timestamp, sizeof(std::uint64_t));
			return *reinterpret_cast<std::uint64_t*>(ptr);
		}
		// ubsan will be abset if we just use reinterpret_cast
	};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(4)
	struct update : erased_update {
		std::uint32_t sequence_number;
	};
#pragma pack(pop)
	const erased_update* last_update() noexcept {
		return nullptr;
	}

	void Example_Ugly(void* p) {
		const auto ptr = last_update();
		if (ptr->length < sizeof(update)) {
			throw std::runtime_error("Update too short!");
		}
		const auto u = start_lifetime_as<update>(ptr);
		std::cout << "Sequence number" << u->sequence_number << std::endl;
	}
	// now let's make Example better
	enum class update_as_error { success, too_short };
	std::error_code make_error_code(update_as_error e) noexcept
	{
		return std::error_code(static_cast<int>(e), std::system_category());
	}

	template<typename T>
	using update_as_result = std::expected<const T*, std::error_code>;

	template<typename T>
	update_as_result<T> update_as(const erased_update& u) noexcept {
		if (u.length < sizeof(T)) {
			const auto ec = make_error_code(update_as_error::too_short);
			return std::unexpected(ec);
		}
		return start_lifetime_as<T>(&u);
	}

	void Example_Better(void* p) {
		auto ptr = last_update();
		const auto u = update_as<update>(*ptr).value();
		ptr = std::launder(ptr);
		std::cout << "Timestamp" << ptr->timestamp() << std::endl;
		std::cout << "Sequence number" << u->sequence_number << std::endl;
	}

#pragma pack(push)
#pragma pack(4)
	struct update2 : erased_update {
		std::uint32_t sequence_number;
		std::string_view name() const noexcept
		{
			const auto size = length - sizeof(update2);
			if (!size) { return {}; }
			const auto ptr = reinterpret_cast<const std::byte*>(this) + sizeof(*this);
			const auto str = start_lifetime_as_array<char>(ptr, size);
			return { str, size };
		}
	};
#pragma pack(pop)


	auto Example3(float* ptr) {
		return std::bit_cast<int>(*ptr);
	}

	// std::bit_cast is likely better than handwritten ones.
	template<class To,class From>
	requires (sizeof(To) == sizeof(From)) &&
		std::is_trivially_copyable_v<To> &&
			std::is_trivially_copyable_v<From>
			To bit_cast_(const From& from) noexcept {
			alignas(To) std::byte buffer[sizeof(To)];
			const auto ptr = std::memcpy(buffer, std::addressof(from), sizeof(To));
			return *reinterpret_cast<To*>(ptr);
		}


}