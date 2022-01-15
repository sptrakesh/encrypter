//
// Created by Rakesh on 2019-05-16.
//

#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include "../../src/log/NanoLog.h"

int main( int argc, char* argv[] )
{
  nanolog::set_log_level( nanolog::LogLevel::DEBUG );
  nanolog::initialize( nanolog::GuaranteedLogger(), "/tmp/", "encrypter-test", false );
  return Catch::Session().run( argc, argv );
}
