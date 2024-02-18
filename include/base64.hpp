#ifndef BASE64_HPP_
#define BASE64_HPP_

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>

namespace base64 {

namespace {

std::array<char, 64> constexpr encode_table{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

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

}  // end anonymous namespace

inline std::string to_base64(std::string_view binaryText) {
  std::string encoded(
      (binaryText.size() / 3 + (binaryText.size() % 3 > 0)) << 2, padding_char);

  const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&binaryText[0]);
  char* currEncoding = &encoded[0];

  union {
    uint32_t temp = 0;
    struct {
      uint32_t first : 6, second : 6, third : 6, fourth : 6;
    } tempBytes;
  } byteunion;

  for (uint32_t i = binaryText.size() / 3; i;
       --i, bytes += 3, currEncoding += 4) {
    byteunion.temp = bytes[0] << 16 | bytes[1] << 8 | bytes[2];

    currEncoding[0] = encode_table[byteunion.tempBytes.fourth];
    currEncoding[1] = encode_table[byteunion.tempBytes.third];
    currEncoding[2] = encode_table[byteunion.tempBytes.second];
    currEncoding[3] = encode_table[byteunion.tempBytes.first];
  }

  switch (binaryText.size() % 3) {
    case 1: {
      byteunion.temp = bytes[0] << 16;

      currEncoding[0] = encode_table[byteunion.tempBytes.fourth];
      currEncoding[1] = encode_table[byteunion.tempBytes.third];
      break;
    }
    case 2: {
      byteunion.temp = bytes[0] << 16 | bytes[1] << 8;

      currEncoding[0] = encode_table[byteunion.tempBytes.fourth];
      currEncoding[1] = encode_table[byteunion.tempBytes.third];
      currEncoding[2] = encode_table[byteunion.tempBytes.second];
    } break;
  case 0: {
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

  uint32_t numPadding = (*std::prev(base64Text.end(), 1) == padding_char) +
                        (*std::prev(base64Text.end(), 2) == padding_char);

  std::string decoded((base64Text.size() * 3 >> 2) - numPadding, '.');

  union {
    uint32_t temp;
    uint8_t tempBytes[4];
  };
  const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&base64Text[0]);
  char* currDecoding = &decoded[0];

  for (uint32_t i = (base64Text.size() >> 2) - (numPadding != 0); i;
       --i, bytes += 4, currDecoding += 3) {
    if (decode_table[bytes[0]] == 0x64 || decode_table[bytes[1]] == 0x64 ||
        decode_table[bytes[2]] == 0x64 || decode_table[bytes[3]] == 0x64) {
      throw std::runtime_error{
          "Invalid base64 encoded data - Invalid character"};
    }

    temp = decode_table[bytes[0]] << 18 | decode_table[bytes[1]] << 12 |
           decode_table[bytes[2]] << 6 | decode_table[bytes[3]];
    currDecoding[0] = static_cast<char>(tempBytes[2]);
    currDecoding[1] = static_cast<char>(tempBytes[1]);
    currDecoding[2] = static_cast<char>(tempBytes[0]);
  }

  switch (numPadding) {
  case 2: {
      temp = decode_table[bytes[0]] << 18 | decode_table[bytes[1]] << 12;
      currDecoding[0] = static_cast<char>(tempBytes[2]);
      break;
  }
  case 1: {
      temp = decode_table[bytes[0]] << 18 | decode_table[bytes[1]] << 12 |
             decode_table[bytes[2]] << 6;
      currDecoding[0] = static_cast<char>(tempBytes[2]);
      currDecoding[1] = static_cast<char>(tempBytes[1]);
      break;
  }
  case 0: {
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
