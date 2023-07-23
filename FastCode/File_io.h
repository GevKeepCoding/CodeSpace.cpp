#pragma once

#include <wtypes.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <filesystem>
#include <cstdio>
#include "Windows.h"

namespace file_io {

	void widnows_sdk_write()
	{
		// create a file.
		HANDLE file = CreateFile(L"test.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		// add  with 10K content into it
		char buf[10240];
		memset(buf, 'A', 10240);
		DWORD written;
		WriteFile(file, buf, 10240, &written, NULL);
		CloseHandle(file);

		// Now do the same as above using c++ filesystem library
		/*std::ofstream file2("test2.txt", std::ios::binary);
		std::vector<char> buf2(10240, 'A');
		file2.write(buf2.data(), buf2.size());
		file2.close();*/
	}


	void windows_sdk_read()
	{
		constexpr size_t buf_size = 4096u;

		char buf[buf_size];

		HANDLE file = CreateFile(L"test.txt", GENERIC_READ,
								 FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (file == INVALID_HANDLE_VALUE)
		{
			// handle error
		}
		// don't ignore return value of Readfile
		auto result = ReadFile(file, buf, buf_size, NULL, NULL);
		if (result == 0)
		{
			
		}
		CloseHandle(file);

		printf(buf);

	}



	void windows_sdk_read_using_File_mapping()
	{
		HANDLE file = CreateFile(L"test.txt", GENERIC_READ,
								 FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		HANDLE mapping = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);
		// check for errors
		if (mapping == NULL)
		{
			// handle error
			return;
		}
	
		LPVOID file_data = MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);
		// do something with file_data
		if (file_data == NULL)
		{
			// handle error
			return;
		}
		printf((char*)file_data);
	}

	int windows_file_mapping_16k() {
		const DWORD dwFileSizeHigh = 4;
		const DWORD dwFileSizeLow = 0;

		// Create a file
		HANDLE hFile = CreateFile(L"LargeFile.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			std::cerr << "Could not create file." << std::endl;
			return 1;
		}

		// Set the file size to 16GB
		LARGE_INTEGER li;
		li.QuadPart = 16LL * 1024 * 1024 * 1024;
		if (!SetFilePointerEx(hFile, li, NULL, FILE_BEGIN)) {
			std::cerr << "Could not set file size." << std::endl;
			return 1;
		}
		if (!SetEndOfFile(hFile)) {
			std::cerr << "Could not set end of file." << std::endl;
			return 1;
		}

		// Create a file mapping object
		HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READWRITE, dwFileSizeHigh, dwFileSizeLow, NULL);
		if (hMapFile == NULL) {
			std::cerr << "Could not create file mapping object." << std::endl;
			return 1;
		}

		// Map the file to the process's address space
		LPVOID lpMapAddress = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (lpMapAddress == NULL) {
			std::cerr << "Could not map view of file." << std::endl;
			return 1;
		}


		// Use the file
		// ...
		{
			for (size_t i = 0; i < 100; i += 2)
			{
				((char*)lpMapAddress)[i] = 'a';
			}

			((char*)lpMapAddress)[li.QuadPart - 1] = 'a';

			// print first 100 characters.
			auto first_100Bytes = std::string_view((char*)lpMapAddress, 100);
			std::cout << first_100Bytes;
		}
		// Clean up
		UnmapViewOfFile(lpMapAddress);
		CloseHandle(hMapFile);
		CloseHandle(hFile);


		// Just in case delete file 
		if (!DeleteFile(L"LargeFile.txt")) {
			std::cerr << "Could not delete file." << std::endl;
			return 1;
		}


		return 0;
	}

	auto file_io_in_cpp = []
	{

		//windows_sdk_read_using_File_mapping();



		return 0;
	}();
}