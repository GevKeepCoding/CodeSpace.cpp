## [Taking a Byte Out of C++ - Avoiding Punning by Starting Lifetimes - Robert Leahy - CppCon 2022](https://www.youtube.com/watch?v=pbkQG09grFw)

* Certain types have "implicit lifetimes".
	- Aggregate types
	- At least one trivial constructor and trivial destructor

* Certain Opterations implicitly create objects of **implicit lifetime** type.
	- std::malloc
	- std::memcpy and ::memmove
	- Starting lifetime of array of char, unsigned char, or std::byte
	- operator new and operator new[]
P0593

---
std::string is not a trivial type, so the following code is not valid.
```cpp
const auto ptr = (std::string*)std::malloc(sizeof(std::string) * 4);
if (!ptr) throw std::bad_alloc();
for (int i = 0; i < 4; ++i) {
	ptr[i] = std::string("Hello");
}
```
We have to use placement new to construct the objects.
```cpp
const auto ptr = (std::string*)std::malloc(sizeof(std::string) * 4);
if (!ptr) throw std::bad_alloc();
for (int i = 0; i < 4; ++i) {
	new (&ptr[i]) std::string("Hello");
}
```
---
Let's see how we can get good code generation and avoid undefined behavior.
```cpp
// old style: good codegan.
int baz(const void* ptr) noexcept {
	return *static_cast<const int*>(ptr);
}
```
```cpp
// option 1: good codegen.
int baz(const void* ptr) noexcept {
    int retr;
	std::memcpy(&retr, ptr, sizeof(int));
	return retr;
}
// implicit lifetime types can be not default constructible.
// now let's see option 2.
int baz(const void* ptr) noexcept {
    alignas(int) std::byte buffer[sizeof(int)];
	const auto retr = std::memcpy(buffer, ptr, sizeof(int));
	return *reinterpret_cast<const int*>(retr);
}

// Modern C++20: has bit_cast for above operations
template <typename To, typename From>
requires (sizeof (To) == sizeof(From)) &&
		 std::is_trivially_copyable_v<From> &&
		 std::is_trivially_copyable_v<To>
To bit_cast(const From& from) noexcept {
	alignas(To) std::byte buffer[sizeof(To)];
	const auto ptr = std::memcpy(buffer, std::addressof(from), sizeof(To));
	return *reinterpret_cast<To*>(ptr);
}

```
Other example of how to benefit from the implicit lifetime types.
```cpp
// old style: good codegen.
void quux(const void* ptr) noexcept {
    corge(*static_cast<const int*>(ptr));)
}

// option 1: bad codegen.
void quux (const void* ptr) noexcept {
     alignas(int) std::byte buffer[sizeof(int)];
	 const auto retr = std::memcpy(buffer, ptr, sizeof(int));
	 corge(*reinterpret_cast<int*>(retr));
}
// because of c++ rules, corge can be comparing the input it gets with the global variable address. so we can't optimize above function.
// Option 2: good codegen.
void quux(const void* ptr) noexcept {
    const auto mutable_ptr = const_cast<void*>(ptr);
	const auto byte_ptr = new (mutable_ptr) std::byte[sizeof(int)];
	const auto int_ptr = reinterpret_cast<const int*>(byte_ptr);
	corge(*int_ptr);
}
// good codegen. but we have to write more code. this is really bad.
// Option 3: good codegen.
// STL has a helper function for this.
template<typename T>
const T* start_lifetime_as(const void* p) noexcept  {
   const auto mp = const_cast<void*>(p);
   const auto bytes = new (mp) std::byte[sizeof(T)];
   const auto ptr = reinterpret_cast<const T*>(bytes);
   (void)*ptr;
   return ptr;
}

// Right Code:
void corge(const int& ) noexcept;
void quux(const void* ptr) noexcept {
	corge(*start_lifetime_as<int>(ptr));
}


```
When working with pages directly mapped to memory we need above techniques to write correct and efficient code.

If you will try to bind the reference to the member of the object and this member is not aligned properly ubsan will be triggered.
```cpp
#pragma pack(push)
#pragma pack(4)
struct erased_update {
    std::uint64_t raw_timestamp;
    std::uint32_t length;
	std::uint64_t timestamp() const noexcept {
	      alignas(std::uint64_t) std::byte buffer[sizeof(std::uint64_t)];
		  const auto ptr = std::memcpy(buffer, std::addressof(raw_timestamp), sizeof(std::uint64_t));
		  return *reinterpret_cast<std::uint64_t*>(ptr);
	}
};
#pragma pack(pop)


#pragma pack(push)
#pragma pack(4)
struct update : erased_update {
  std::uint32_t sequence_number;
};

#pragma pack(pop)

const auto ptr = q.last_update();
if (ptr->length < sizeof(update)) {
throw std::runtime_error("Invalid update");
}

const auto u = start_lifetime_as<update>(ptr);
std::cout << "Sequence Number" << u->sequence_number << std::endl;

```


