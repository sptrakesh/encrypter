//
// Created by Rakesh on 27/10/2020.
//

#include <catch2/catch.hpp>
#include "../../src/lib/util/encrypter.hpp"

namespace test::aes256
{
  std::string encrypted;
}

SCENARIO("AES 256 CBC encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::AES_256_CBC, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::aes256::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::aes256::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::aes256::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}

SCENARIO("AES 256 CFB encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::AES_256_CFB, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::aes256::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::aes256::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::aes256::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}

SCENARIO("AES 256 ECB encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::AES_256_ECB, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::aes256::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::aes256::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::aes256::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}

SCENARIO("AES 256 OFB encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::AES_256_OFB, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::aes256::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::aes256::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::aes256::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}
