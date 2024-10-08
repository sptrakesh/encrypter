//
// Created by Rakesh on 27/10/2020.
//

#include <catch2/catch.hpp>
#include "../../src/lib/util/encrypter.hpp"

namespace test::blowfish
{
  std::string encrypted;
}

SCENARIO("Blowfish CBC encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::BLOWFISH_CBC, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::blowfish::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::blowfish::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::blowfish::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}

SCENARIO("Blowfish CFB encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::BLOWFISH_CFB, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::blowfish::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::blowfish::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::blowfish::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}

SCENARIO("Blowfish ECB encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::BLOWFISH_ECB, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::blowfish::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::blowfish::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::blowfish::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}

SCENARIO("Blowfish OFB encryption/decryption test suite")
{
  GIVEN("Encrypter instance with an encryption key")
  {
    using spt::util::Encrypter;
    auto encrypter = Encrypter{ Encrypter::BLOWFISH_OFB, "unit test secret" };
    const auto text = std::string{ "Unit test string to be encrypted and decrypted" };

    WHEN("Encrypting a string")
    {
      test::blowfish::encrypted = encrypter.encrypt( text );
      REQUIRE_FALSE(test::blowfish::encrypted.empty());
    }

    AND_THEN("Decrypting encrypted string gives back original string" )
    {
      const auto dec = encrypter.decrypt(test::blowfish::encrypted);
      REQUIRE(dec.size() == text.size());
      REQUIRE(dec == text);
    }
  }
}
