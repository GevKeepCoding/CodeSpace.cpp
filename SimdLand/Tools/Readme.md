
 Some compiler flags:
 g++ file.cpp -I. -g -O3 -Wall -Wextra -Werror -pedantic -mavx2 --std=c++latest -c -o file.o

## Linux command to see the assembly code of a obj file:
* objdump -d -M intel -S file.o
* objdump -d --demangle file.o

