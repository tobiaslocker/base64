# base64
A simple approach to convert strings from and to base64.
Header only library.

## Usage

```cpp
#include <iostream>

#include "base64.hpp"

int main() {
  auto base64= to_base64("Hello, World!");
  std::cout << base64 << std::endl; // SGVsbG8sIFdvcmxkIQ==
  auto s = from_base64("SGVsbG8sIFdvcmxkIQ==");
  std::cout << s << std::endl; // Hello, World!
}
```

## Notes
This library relies on C++17. 

A different, unrelated C++20 library for base64 encoding/decoding can be found at https://github.com/matheusgomes28/base64pp
