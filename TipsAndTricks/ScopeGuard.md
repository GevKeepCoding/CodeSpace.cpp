## Folly has a ScopeGuard class.

* It can be used to run a function when the ScopeGuard goes out of scope. This is useful for running cleanup code when a function exits, even if the function exits early due to an exception.

```cpp
#include <folly/ScopeGuard.h>
#include <iostream>

void foo() {
  std::cout << "foo\n";
  SCOPE_EXIT {
	std::cout << "cleanup\n";
  };
  throw std::runtime_error("error");
}




```

