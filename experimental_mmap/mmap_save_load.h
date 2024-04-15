#include <filesystem> // Required for std::filesystem::exists

// Use boost library to create a a memory mapped file and serialize std::map<std::string, std::string> to it.
#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>


#include <iostream>
#include <chrono>





namespace bi = boost::interprocess;


// Define an allocator for the string type
typedef bi::allocator<char, bi::managed_mapped_file::segment_manager> CharAllocator;
typedef bi::basic_string<char, std::char_traits<char>, CharAllocator> MyString;

struct Person1 {
  int age;
  MyString name;
  Person1(int a, const MyString& n) : age(a), name(n) {}
};

// Define a map type with the custom string type as key and value
typedef std::pair<const MyString, Person1> ValueType;
typedef bi::allocator<ValueType, bi::managed_mapped_file::segment_manager> ValueAllocator;
typedef bi::map<MyString, Person1, std::less<MyString>, ValueAllocator> MyMap;



void save_to_mapped_file(const char* file_name, size_t size = 2ull * 1024 * 1024 * 1024) {

  // Create a managed mapped file with 2GB size
  auto mapped_file = bi::managed_mapped_file(bi::create_only, file_name, size);

  // Create a map in the managed mapped file
  MyMap* my_map = mapped_file.construct<MyMap>("MyMap")(mapped_file.get_segment_manager());
  
  // Insert some data into the map 10000 times
  for (int i = 0; i < 10000; ++i) {
    std::string idx = std::to_string(i);
    my_map->insert(ValueType(MyString(idx.c_str(), mapped_file.get_segment_manager()), Person1(i, MyString("James", mapped_file.get_segment_manager())) ));
  }
}

// Function to read above created mapped file
void read_mapped_file(const char* file_name) {
  using namespace boost::interprocess;

  // Open the managed mapped file
  managed_mapped_file mapped_file(open_only, file_name);

  // Find the map in the managed mapped file
  MyMap* my_map = mapped_file.find<MyMap>("MyMap").first;

  // Access the data in the map
  for (const auto& pair : *my_map) {
    std::cout << pair.first.c_str() << ": " << pair.second.name.c_str() << ", " << pair.second.age << std::endl;
  }
}

void read_write_mmap() {
  const char* file_name = "mapped_file.bin";
  // remove file_name if exists
  if (std::filesystem::exists(file_name)) {
    std::cout << "File already exists, removing it." << std::endl;
    remove(file_name);
  }
  // use std::chrono to time below 2 function calls.
  auto start = std::chrono::high_resolution_clock::now();
  save_to_mapped_file(file_name);
  read_mapped_file(file_name);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
}
