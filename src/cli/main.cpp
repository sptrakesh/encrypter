//
// Created by Rakesh on 27/10/2020.
//

#include "../lib/util/clara.h"
#include "../lib/util/encrypter.h"
#include "../log/NanoLog.h"

#include <cstdlib>
#include <iostream>

std::string key()
{
  char* val = std::getenv( "ENCRYPTION_KEY" );
  return val == nullptr ? std::string( "rFmHPdwX5E/2gq2fSuPrgSDUi4W/+Y7/QL3NxINM+pY=" ) : std::string( val );
}

int main( int argc, char const * const * argv )
{
  using spt::util::Encrypter;
  using clara::Opt;
  using clara::Arg;

  std::string input;
  std::string logLevel{ "info" };
  std::string dir{ "/tmp/" };
  bool encrypt = false;
  bool decrypt = false;
  bool help = false;

  auto options = clara::Help( help ) |
      Opt( encrypt)["-e"]["--encrypt"](
          "Encrypt input text. Either this or decrypt must be specified." ) |
      Opt( decrypt )["-d"]["--decrypt"](
          "Decrypt input text. Either this of encrypt must be specified." ) |
      Opt(logLevel, "info")["-l"]["--log-level"]("Log level to use [debug|info|warn|critical] (default info).") |
      Opt(dir, "/tmp/")["-o"]["--log-dir"]("Log directory (default /tmp/)") |
      Arg( input, "input" )( "Text to encrypt or decrypt." );

      auto result = options.parse( clara::Args( argc, argv ));
  if ( !result )
  {
    std::cerr << "Error in command line: " << result.errorMessage()
              << std::endl;
    exit( 1 );
  }

  if ( help )
  {
    options.writeToStream( std::cout );
    exit( 0 );
  }

  if ( !encrypt && !decrypt )
  {
    options.writeToStream( std::cout );
    exit( 1 );
  }

  if ( input.empty() )
  {
    options.writeToStream( std::cout );
    exit( 2 );
  }

  if ( logLevel == "debug" ) nanolog::set_log_level( nanolog::LogLevel::DEBUG );
  else if ( logLevel == "info" ) nanolog::set_log_level( nanolog::LogLevel::INFO );
  else if ( logLevel == "warn" ) nanolog::set_log_level( nanolog::LogLevel::WARN );
  else if ( logLevel == "critical" ) nanolog::set_log_level( nanolog::LogLevel::CRIT );
  nanolog::initialize( nanolog::GuaranteedLogger(), dir, "encrypter-cli", false );

  auto encrypter = Encrypter( Encrypter::AES_256_CBC, key() );
  if (encrypt)
  {
    const auto encrypted = encrypter.encrypt( input );
    std::cout << encrypted << std::endl;
  }

  if (decrypt)
  {
    const auto dec = encrypter.decrypt( input );
    std::cout << dec << std::endl;
  }
}