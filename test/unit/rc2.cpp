//
// Created by Rakesh on 27/10/2020.
//

#include <catch2/catch.hpp>
#include "../../src/lib/util/encrypter.hpp"

namespace test::rc2
{
  std::string encrypted;
}

SCENARIO("RC2 CBC encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::RC2_CBC, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::rc2::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::rc2::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::rc2::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}

SCENARIO("RC2 CFB encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::RC2_CFB, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::rc2::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::rc2::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::rc2::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}

SCENARIO("RC2 ECB encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::RC2_ECB, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::rc2::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::rc2::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::rc2::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}

SCENARIO("RC2 OFB encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::RC2_OFB, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::rc2::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::rc2::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::rc2::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}
