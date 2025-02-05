// Test suite ported
// https://github.com/client9/stringencoders/blob/master/test/modp_b64_test.c
#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "../include/base64.hpp"

// NOLINTNEXTLINE
TEST(Base64Encode, Padding) {
  // Test 1-6 bytes input and decode
  {
    /* 1 in, 4 out */
    std::string const input{1};
    auto const encoded{base64::to_base64(input)};
    ASSERT_EQ(4, encoded.size());
    auto const decoded{base64::from_base64(encoded)};
    ASSERT_EQ(input, decoded);
  }
  {
    /* 2 in, 4 out */
    std::string const input{1, 1};
    auto const encoded{base64::to_base64(input)};
    ASSERT_EQ(4, encoded.size());
    auto const decoded{base64::from_base64(encoded)};
    ASSERT_EQ(input, decoded);
  }
  {
    /* 3 in, 4 out */
    std::string const input{1, 1, 1};
    auto const encoded{base64::to_base64(input)};
    ASSERT_EQ(4, encoded.size());
    auto const decoded{base64::from_base64(encoded)};
    ASSERT_EQ(input, decoded);
  }
  {
    /* 4 in, 8 out */
    std::string const input{1, 1, 1, 1};
    auto const encoded{base64::to_base64(input)};
    ASSERT_EQ(8, encoded.size());
    auto const decoded{base64::from_base64(encoded)};
    ASSERT_EQ(input, decoded);
  }
  {
    /* 5 in, 8 out */
    std::string const input{1, 1, 1, 1, 1};
    auto const encoded{base64::to_base64(input)};
    ASSERT_EQ(8, encoded.size());
    auto const decoded{base64::from_base64(encoded)};
    ASSERT_EQ(input, decoded);
  }
  {
    /* 6 in, 8 out */
    std::string const input{1, 1, 1, 1, 1, 6};
    auto const encoded{base64::to_base64(input)};
    ASSERT_EQ(8, encoded.size());
    auto const decoded{base64::from_base64(encoded)};
    ASSERT_EQ(input, decoded);
  }
}

// NOLINTNEXTLINE
TEST(Base64Encode, EncodeDecode) {
  // Test all 17M 3 bytes inputs to encoder, decode
  // and make sure it's equal.
  for (int i = 0; i < 256; ++i) {
    for (int j = 0; j < 256; ++j) {
      for (int k = 0; k < 256; ++k) {
        std::string const input{static_cast<char>(i), static_cast<char>(j),
                                static_cast<char>(k)};
        auto const encoded{base64::to_base64(input)};
        ASSERT_EQ(4, encoded.size());
        auto const decoded{base64::from_base64(encoded)};
        ASSERT_EQ(input, decoded);
      }
    }
  }
}

// NOLINTNEXTLINE
TEST(Base64Encode, DecodeErrors) {
  {
    // test bad input -  all combinations
    char goodchar = 'A';
    char badchar = '~';
    std::array<std::uint8_t, 4> decode;
    for (int i = 1; i < 16; ++i) {
      decode[0] = static_cast<char>(((i & 0x01) == 0) ? goodchar : badchar);
      decode[1] = static_cast<char>(((i & 0x02) == 0) ? goodchar : badchar);
      decode[2] = static_cast<char>(((i & 0x04) == 0) ? goodchar : badchar);
      decode[3] = static_cast<char>(((i & 0x08) == 0) ? goodchar : badchar);

      ASSERT_THROW(
          base64::decode_into<std::string>(decode.begin(), decode.end()),
          std::runtime_error);
    }
  }
  {
    // test just 1-4 padchars
    for (int i = 1; i <= 4; ++i) {
      std::vector<char> decode(i, '=');
      ASSERT_THROW(
          base64::decode_into<std::string>(decode.begin(), decode.end()),
          std::runtime_error);
    }
  }
  {
    // Test good+3 pad chars (should be impossible)
    std::string decode("A===");
    ASSERT_THROW(base64::decode_into<std::string>(decode.begin(), decode.end()),
                 std::runtime_error);
  }
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
