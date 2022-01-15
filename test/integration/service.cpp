//
// Created by Rakesh on 13/01/2022.
//

#include <catch2/catch.hpp>

#include "../../src/log/NanoLog.h"
#include "../../src/api/api.h"

namespace spt::encrypter::itest::connection
{
  std::string encrypted;
}

using namespace std::string_view_literals;
namespace api = spt::encrypter::api;

SCENARIO( "TCP service test", "tcp" )
{
  GIVEN( "Using the client API" )
  {
    const auto text = "Test string to be encrypted and decrypted"sv;

    WHEN( "Encrypting a string" )
    {
      auto enc = api::encrypt( text );
      REQUIRE_FALSE( enc.empty() );
      spt::encrypter::itest::connection::encrypted = enc;
    }

    AND_WHEN( "Decrypting a string" )
    {
      REQUIRE_FALSE( spt::encrypter::itest::connection::encrypted.empty() );
      const auto dec = api::decrypt( spt::encrypter::itest::connection::encrypted );
      REQUIRE( dec.size() == text.size() );
      REQUIRE( dec == text );
    }

    AND_WHEN( "Encrypting and decrypting within same context" )
    {
      const auto another = "0ENKfo4NkIAHVBoNUtUseShHK/6JQ11YGALUSA/RLcICLQaTyxDJcldEsxxM0h+AWdkXAXNY6xOhCk+TkobTayscjTPqu6bzKIxnD1hso8Hz/ZWsh4SlSxHLhHAZRg6VfBZlITexMDdMOks2eauDdND6ft5kT+7uJFpq5EfnuHE="sv;
      auto enc = api::encrypt( another );
      REQUIRE_FALSE( enc.empty() );
      const auto dec = api::decrypt( enc );
      REQUIRE( dec.size() == another.size() );
      REQUIRE( dec == another );
    }

    AND_WHEN( "Encrypting and decrypting in a loop" )
    {
      const auto another = R"(VvgRpEXRtq3AMUDdIO7GvFDfHgqZFC/n5zh72V7HEGn9rNIXTYbHGwGVjoSEev6h
dE6EezzqeeshfkDEO/Qqs1Csvzf+QrzqCaQOq40LuhpDdJym2SnuIGJoPDnuTu7/
Fu2DjHFSibtC/E1UokF6hwd5Wiq35BiV3Yzev99DTWs=)"sv;

      for ( auto i = 0; i < 10; ++i )
      {
        auto enc = api::encrypt( another );
        REQUIRE_FALSE( enc.empty() );
        const auto dec = api::decrypt( enc );
        REQUIRE( dec.size() == another.size() );
        REQUIRE( dec == another );
      }
    }
  }
}