# base64

A simple, header-only C++17 library for Base64 encoding and decoding. Uses modern C++ features when available
(e.g., `std::bit_cast` in C++20) while remaining portable and efficient.

## Features

- Header-only, single-file library  
- Fast encoding/decoding using lookup tables  
- Safe type punning using `std::bit_cast` (avoids undefined behavior with unions)  
- Throws `std::runtime_error` for invalid Base64 input (size, padding, or characters)  

## Platform Support

- Compilers: GCC, Clang, MSVC  
- Architectures: x86, x64, ARM, AArch64 (little-endian)  
- Requires C++17 (C++20 features optional)  

## Usage

```cpp
#include <iostream>  
#include "base64.hpp"  

int main() {  
    auto encoded = base64::to_base64("Hello, World!");  
    std::cout << encoded << std::endl; // SGVsbG8sIFdvcmxkIQ==  

    auto decoded = base64::from_base64("SGVsbG8sIFdvcmxkIQ==");  
    std::cout << decoded << std::endl; // Hello, World!  
}
```

## Notes

- Inspired by Nick Galbreath's modp_b64 (used by Chromium) for high performance  
- Compatible with C++17; optionally uses C++20 features  
- Avoids union-based type punning for safety  
- Faster implementations exist using SIMD or multithreading but are not header-only  

## References

- Benchmark of C/C++ Base64 libraries: https://github.com/gaspardpetit/base64/  
- Chromium modp_b64: https://github.com/chromium/chromium/tree/main/third_party/modp_b64  
- SIMD/optimized alternatives:  
  - https://github.com/aklomp/base64  
  - https://github.com/simdutf/simdutf  
  - https://github.com/powturbo/Turbo-Base64
