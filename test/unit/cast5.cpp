//
// Created by Rakesh on 27/10/2020.
//

#include <catch2/catch.hpp>
#include "../../src/lib/util/encrypter.h"

namespace test::cast5
{
  std::string encrypted;
}

SCENARIO("Cast5 CBC encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::CAST5_CBC, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::cast5::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::cast5::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::cast5::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}

/*
SCENARIO("Cast5 CFB encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::CAST5_CFB, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::cast5::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::cast5::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::cast5::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}

SCENARIO("Cast5 ECB encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::CAST5_ECB, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::cast5::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::cast5::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::cast5::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}

SCENARIO("Cast5 OFB encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::CAST5_OFB, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::cast5::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::cast5::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::cast5::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}
 */
