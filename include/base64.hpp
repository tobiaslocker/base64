#include <algorithm>
#include <string>

#include <bitset>
#include <iostream>

std::string to_base64(std::string const &data) {
  int counter = 0;
  uint32_t bit_stream = 0;

  std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "0123456789+/";

  std::string base64;
  int offset = 0;
  std::for_each(data.begin(), data.end(), [&](unsigned char c) {
    auto num_val = static_cast<unsigned int>(c);
    offset = 16 - counter % 3 * 8;
    bit_stream += num_val << offset;

    if (offset == 16) {
      base64 += base64_chars.at(bit_stream >> 18 & 0x3f);
    }

    if (offset == 8) {
      base64 += base64_chars.at(bit_stream >> 12 & 0x3f);
    }
    if (offset == 0 && counter != 3) {
      base64 += base64_chars.at(bit_stream >> 6 & 0x3f);
      base64 += base64_chars.at(bit_stream & 0x3f);
      bit_stream = 0;
    }
    counter++;
  });

  if (offset == 16) {
    base64 += base64_chars.at(bit_stream >> 12 & 0x3f);
    base64 += '=';
    base64 += '=';
  }
  if (offset == 8) {
    base64 += base64_chars.at(bit_stream >> 6 & 0x3f);
    base64 += '=';
  }
  return base64;
}
