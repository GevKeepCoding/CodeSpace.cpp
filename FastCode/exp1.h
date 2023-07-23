#pragma once

#include <algorithm>


/*

template<class T>
uninitialized_trivial_destructive_move(T* from, T* to)

above function is required to memcpy and not even call destructor of input element

This kind of code can make many copy move algos much faster.

----
*/


