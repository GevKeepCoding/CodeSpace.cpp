#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "person.pb.h" // Include the generated header

int protobuf_save_load() {

  Person person;
  person.set_age(30);
  person.set_name("John Doe");

  // --- Serialization ---
  std::string serialized_str;
  if (!person.SerializeToString(&serialized_str)) {
    std::cerr << "Failed to serialize person." << std::endl;
    return 1;
  }
  std::cout << "Serialized data: " << serialized_str << std::endl; // Note: This might not be human-readable

  // Optional: Serialize to a file
  std::string filename = "person_data.bin";
  std::fstream output(filename, std::ios::out | std::ios::trunc | std::ios::binary);
  if (!person.SerializeToOstream(&output)) {
    std::cerr << "Failed to write person to file." << std::endl;
    return 1;
  }
  output.close(); // Close the file stream
  std::cout << "Serialized data written to " << filename << std::endl;


  // --- Deserialization ---
  Person deserialized_person_from_str;
  if (!deserialized_person_from_str.ParseFromString(serialized_str)) {
    std::cerr << "Failed to deserialize person from string." << std::endl;
    return 1;
  }

  std::cout << "\nDeserialized from string:" << std::endl;
  std::cout << "Name: " << deserialized_person_from_str.name() << std::endl;
  std::cout << "Age: " << deserialized_person_from_str.age() << std::endl;

  // Optional: Deserialize from the file
  Person deserialized_person_from_file;
  std::fstream input(filename, std::ios::in | std::ios::binary);
  if (!input) {
    std::cerr << filename << ": File not found. Creating a new file." << std::endl;
  }
  else if (!deserialized_person_from_file.ParseFromIstream(&input)) {
    std::cerr << "Failed to read person from file." << std::endl;
    return 1;
  }
  input.close(); // Close the file stream

  if (deserialized_person_from_file.IsInitialized()) {
    std::cout << "\nDeserialized from file:" << std::endl;
    std::cout << "Name: " << deserialized_person_from_file.name() << std::endl;
    std::cout << "Age: " << deserialized_person_from_file.age() << std::endl;
  }


  // Clean up Protocol Buffers library (optional, but good practice)
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}