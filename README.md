# base64
A simple approach to convert strings from and to base64.
Header only library.

## Usage

```cpp
#include "base64.hpp"
#include <iostream>

int main() {
  auto base64= to_base64("Hello, World!");
  std::cout << base64 << '\n'; // SGVsbG8sIFdvcmxkIQ==
  auto s = from_base64("SGVsbG8sIFdvcmxkIQ==");
  std::cout << s << '\n'; // Hello, World!
}
```
