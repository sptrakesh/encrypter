//
// Created by Rakesh on 2019-05-16.
//

#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include "../../src/api/api.hpp"
#include "../../src/log/NanoLog.hpp"

int main( int argc, char* argv[] )
{
  nanolog::set_log_level( nanolog::LogLevel::DEBUG );
  nanolog::initialize( nanolog::GuaranteedLogger(), "/tmp/", "encrypter-itest", false );
  spt::encrypter::api::init( "localhost", "2030" );
  return Catch::Session().run( argc, argv );
}
