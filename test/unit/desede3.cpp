//
// Created by Rakesh on 27/10/2020.
//

#include <catch2/catch.hpp>
#include "../../src/lib/util/encrypter.hpp"

namespace test::desede3
{
  std::string encrypted;
}

SCENARIO("DES EDE3 CBC encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::DES_EDE3_CBC, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::desede3::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::desede3::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::desede3::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}

SCENARIO("DES EDE3 CFB encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::DES_EDE3_CFB, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::desede3::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::desede3::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::desede3::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}

SCENARIO("DES EDE3 ECB encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::DES_EDE3_ECB, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::desede3::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::desede3::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::desede3::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}

SCENARIO("DES EDE3 OFB encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::DES_EDE3_OFB, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::desede3::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::desede3::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::desede3::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}
