#ifndef BASE_64_HPP
#define BASE_64_HPP

#include <algorithm>
#include <stdexcept>
#include <string>

namespace base64 {

inline constexpr std::string_view base64_chars{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                               "abcdefghijklmnopqrstuvwxyz"
                                               "0123456789+/"};

inline std::string to_base64(std::string const &data) {
  size_t counter = 0;
  uint32_t bit_stream = 0;
  size_t offset = 0;
  std::string encoded;
  for (unsigned char c : data) {
    auto num_val = static_cast<unsigned int>(c);
    offset = 16 - counter % 3 * 8;
    bit_stream += num_val << offset;
    if (offset == 16) {
      encoded += base64_chars.at(bit_stream >> 18 & 0x3f);
    }
    if (offset == 8) {
      encoded += base64_chars.at(bit_stream >> 12 & 0x3f);
    }
    if (offset == 0 && counter != 3) {
      encoded += base64_chars.at(bit_stream >> 6 & 0x3f);
      encoded += base64_chars.at(bit_stream & 0x3f);
      bit_stream = 0;
    }
    counter++;
  }
  if (offset == 16) {
    encoded += base64_chars.at(bit_stream >> 12 & 0x3f);
    encoded += "==";
  }
  if (offset == 8) {
    encoded += base64_chars.at(bit_stream >> 6 & 0x3f);
    encoded += '=';
  }
  return encoded;
}

template<class OutputBuffer>
inline OutputBuffer decode_into(std::string_view data) {
	using value_type = typename OutputBuffer::value_type;
	static_assert(std::is_same_v<value_type, char>
		|| std::is_same_v<value_type, unsigned char>
		|| std::is_same_v<value_type, std::byte>);

  size_t counter = 0;
  uint32_t bit_stream = 0;
  OutputBuffer decoded;
  for (unsigned char c : data) {
    auto const num_val = base64_chars.find(c);
    if (num_val != std::string::npos) {
      auto const offset = 18 - counter % 4 * 6;
      bit_stream += static_cast<uint32_t>(num_val) << offset;
      if (offset == 12) {
        decoded.push_back(static_cast<value_type>(bit_stream >> 16 & 0xff));
      }
      if (offset == 6) {
        decoded.push_back(static_cast<value_type>(bit_stream >> 8 & 0xff));
      }
      if (offset == 0 && counter != 4) {
        decoded.push_back(static_cast<value_type>(bit_stream & 0xff));
        bit_stream = 0;
      }
    } else if (c != '=') {
      throw std::runtime_error{"Invalid base64 encoded data"};
    }
    counter++;
  }
  return decoded;
}

inline std::string from_base64(std::string_view data) {
	return decode_into<std::string>(data);
}

} // namespace base64

#endif // BASE_64_HPP
