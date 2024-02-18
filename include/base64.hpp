#ifndef BASE64_HPP_
#define BASE64_HPP_

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <string_view>

#if defined(__cpp_lib_bit_cast)
#include <bit>  // For std::bit_cast.
#endif

namespace base64 {

namespace detail {

#if defined(__cpp_lib_bit_cast)
using std::bit_cast
#else
template <class To, class From>
std::enable_if_t<sizeof(To) == sizeof(From) &&
                     std::is_trivially_copyable_v<From> &&
                     std::is_trivially_copyable_v<To>,
                 To>
bit_cast(const From& src) noexcept {
  static_assert(std::is_trivially_constructible_v<To>,
                "This implementation additionally requires "
                "destination type to be trivially constructible");

  To dst;
  std::memcpy(&dst, &src, sizeof(To));
  return dst;
}
#endif

    inline constexpr char padding_char{'='};

std::array<std::uint8_t, 256> constexpr decode_table{
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x3E, 0x64, 0x64, 0x64, 0x3F,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12,
    0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24,
    0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
    0x31, 0x32, 0x33, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64};

std::array<char, 256> constexpr encode_table_0 = {
    'A', 'A', 'A', 'A', 'B', 'B', 'B', 'B', 'C', 'C', 'C', 'C', 'D', 'D', 'D',
    'D', 'E', 'E', 'E', 'E', 'F', 'F', 'F', 'F', 'G', 'G', 'G', 'G', 'H', 'H',
    'H', 'H', 'I', 'I', 'I', 'I', 'J', 'J', 'J', 'J', 'K', 'K', 'K', 'K', 'L',
    'L', 'L', 'L', 'M', 'M', 'M', 'M', 'N', 'N', 'N', 'N', 'O', 'O', 'O', 'O',
    'P', 'P', 'P', 'P', 'Q', 'Q', 'Q', 'Q', 'R', 'R', 'R', 'R', 'S', 'S', 'S',
    'S', 'T', 'T', 'T', 'T', 'U', 'U', 'U', 'U', 'V', 'V', 'V', 'V', 'W', 'W',
    'W', 'W', 'X', 'X', 'X', 'X', 'Y', 'Y', 'Y', 'Y', 'Z', 'Z', 'Z', 'Z', 'a',
    'a', 'a', 'a', 'b', 'b', 'b', 'b', 'c', 'c', 'c', 'c', 'd', 'd', 'd', 'd',
    'e', 'e', 'e', 'e', 'f', 'f', 'f', 'f', 'g', 'g', 'g', 'g', 'h', 'h', 'h',
    'h', 'i', 'i', 'i', 'i', 'j', 'j', 'j', 'j', 'k', 'k', 'k', 'k', 'l', 'l',
    'l', 'l', 'm', 'm', 'm', 'm', 'n', 'n', 'n', 'n', 'o', 'o', 'o', 'o', 'p',
    'p', 'p', 'p', 'q', 'q', 'q', 'q', 'r', 'r', 'r', 'r', 's', 's', 's', 's',
    't', 't', 't', 't', 'u', 'u', 'u', 'u', 'v', 'v', 'v', 'v', 'w', 'w', 'w',
    'w', 'x', 'x', 'x', 'x', 'y', 'y', 'y', 'y', 'z', 'z', 'z', 'z', '0', '0',
    '0', '0', '1', '1', '1', '1', '2', '2', '2', '2', '3', '3', '3', '3', '4',
    '4', '4', '4', '5', '5', '5', '5', '6', '6', '6', '6', '7', '7', '7', '7',
    '8', '8', '8', '8', '9', '9', '9', '9', '+', '+', '+', '+', '/', '/', '/',
    '/'};

std::array<char, 256> constexpr encode_table_1 = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
    'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
    't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '+', '/', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
    'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
    'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
    'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
    'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', 'A', 'B', 'C',
    'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
    'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
    'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+',
    '/'};

}  // namespace detail

inline std::string to_base64(std::string_view binaryText) {
  std::string encoded((binaryText.size() / 3 + (binaryText.size() % 3 > 0))
                          << 2,
                      detail::padding_char);

  const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&binaryText[0]);
  char* currEncoding = &encoded[0];

  for (uint32_t i = binaryText.size() / 3; i; --i) {
    const uint8_t t1 = *bytes++;
    const uint8_t t2 = *bytes++;
    const uint8_t t3 = *bytes++;
    *currEncoding++ = detail::encode_table_0[t1];
    *currEncoding++ =
        detail::encode_table_1[((t1 & 0x03) << 4) | ((t2 >> 4) & 0x0F)];
    *currEncoding++ =
        detail::encode_table_1[((t2 & 0x0F) << 2) | ((t3 >> 6) & 0x03)];
    *currEncoding++ = detail::encode_table_1[t3];
  }

  switch (binaryText.size() % 3) {
    case 0: {
      break;
    }
    case 1: {
      const uint8_t t1 = bytes[0];
      *currEncoding++ = detail::encode_table_0[t1];
      *currEncoding++ = detail::encode_table_1[(t1 & 0x03) << 4];
      // *currEncoding++ = detail::padding_char;
      // *currEncoding++ = detail::padding_char;
      break;
    }
    case 2: {
      const uint8_t t1 = bytes[0];
      const uint8_t t2 = bytes[1];
      *currEncoding++ = detail::encode_table_0[t1];
      *currEncoding++ =
          detail::encode_table_1[((t1 & 0x03) << 4) | ((t2 >> 4) & 0x0F)];
      *currEncoding++ = detail::encode_table_1[(t2 & 0x0F) << 2];
      // *currEncoding++ = detail::padding_char;
      break;
    }
    default: {
      throw std::runtime_error{"Invalid base64 encoded data"};
    }
  }

  return encoded;
}

template <class OutputBuffer, class InputIterator>
inline OutputBuffer encode_into(InputIterator begin, InputIterator end) {
  std::string_view::const_pointer bptr =
      reinterpret_cast<std::string_view::const_pointer>(&*begin);
  std::string_view::size_type size = end - begin;
  std::string_view str = std::string_view(bptr, size);
  std::string out = to_base64(str);
  return OutputBuffer{out.begin(), out.end()};
}

inline std::string from_base64(std::string_view base64Text) {
  if (base64Text.empty()) return "";

  if ((base64Text.size() & 3) != 0) {
    throw std::runtime_error{
        "Invalid base64 encoded data - Size not divisible by 4"};
  }

  const size_t numlasteqs =
      std::count(base64Text.rbegin(), base64Text.rbegin() + 4, '=');
  if (numlasteqs > 2) {
    throw std::runtime_error{
        "Invalid base64 encoded data - Found more than 2 padding signs"};
  }

  uint32_t numPadding =
      (*std::prev(base64Text.end(), 1) == detail::padding_char) +
      (*std::prev(base64Text.end(), 2) == detail::padding_char);

  std::string decoded((base64Text.size() * 3 >> 2) - numPadding, '.');

  const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&base64Text[0]);
  char* currDecoding = &decoded[0];

  for (uint32_t i = (base64Text.size() >> 2) - (numPadding != 0); i; --i) {
    const uint8_t t1 = *bytes++;
    const uint8_t t2 = *bytes++;
    const uint8_t t3 = *bytes++;
    const uint8_t t4 = *bytes++;

    const uint8_t d1 = detail::decode_table[t1];
    const uint8_t d2 = detail::decode_table[t2];
    const uint8_t d3 = detail::decode_table[t3];
    const uint8_t d4 = detail::decode_table[t4];

    if (d1 == 0x64 || d2 == 0x64 || d3 == 0x64 || d4 == 0x64) {
      throw std::runtime_error{
          "Invalid base64 encoded data - Invalid character"};
    }

    const uint32_t temp = d1 << 18 | d2 << 12 | d3 << 6 | d4;
    const std::array<char, 4> tempBytes =
        detail::bit_cast<std::array<char, 4>, uint32_t>(temp);
    *currDecoding++ = tempBytes[2];
    *currDecoding++ = tempBytes[1];
    *currDecoding++ = tempBytes[0];
  }

  switch (numPadding) {
    case 0: {
      break;
    }
    case 1: {
      const uint8_t t1 = *bytes++;
      const uint8_t t2 = *bytes++;
      const uint8_t t3 = *bytes++;

      const uint8_t d1 = detail::decode_table[t1];
      const uint8_t d2 = detail::decode_table[t2];
      const uint8_t d3 = detail::decode_table[t3];

      if (d1 == 0x64 || d2 == 0x64 || d3 == 0x64) {
        throw std::runtime_error{
            "Invalid base64 encoded data - Invalid character"};
      }

      const uint32_t temp = d1 << 18 | d2 << 12 | d3 << 6;
      const std::array<char, 4> tempBytes =
          detail::bit_cast<std::array<char, 4>, uint32_t>(temp);
      *currDecoding++ = tempBytes[2];
      *currDecoding++ = tempBytes[1];
      break;
    }
    case 2: {
      const uint8_t t1 = *bytes++;
      const uint8_t t2 = *bytes++;

      const uint8_t d1 = detail::decode_table[t1];
      const uint8_t d2 = detail::decode_table[t2];

      if (d1 == 0x64 || d2 == 0x64) {
        throw std::runtime_error{
            "Invalid base64 encoded data - Invalid character"};
      }

      const uint32_t temp = d1 << 18 | d2 << 12;
      const std::array<char, 4> tempBytes =
          detail::bit_cast<std::array<char, 4>, uint32_t>(temp);
      *currDecoding++ = tempBytes[2];
      break;
    }
    default: {
      throw std::runtime_error{
          "Invalid base64 encoded data - Invalid padding number"};
    }
  }

  return decoded;
}

template <class OutputBuffer>
inline OutputBuffer decode_into(std::string_view data) {
  std::string out = from_base64(data);
  return OutputBuffer{out.begin(), out.end()};
}

}  // namespace base64

#endif  // BASE64_HPP_
