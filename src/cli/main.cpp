//
// Created by Rakesh on 27/10/2020.
//

#include "../api/contextholder.hpp"
#include "../api/tcp/connection.hpp"
#include "../lib/util/clara.hpp"
#include "../lib/util/encrypter.hpp"
#include "../log/NanoLog.hpp"

#include <cstdlib>
#include <iostream>

std::string key()
{
  char* val = std::getenv( "ENCRYPTION_KEY" ); // flawfinder: ignore
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
  std::string server;
  std::string port{ "2030" };
  bool encrypt = false;
  bool decrypt = false;
  bool help = false;

  auto options = clara::Help( help ) |
      Opt(server, "localhost")["-s"]["--server"]("Specify service to connect to.  If not specified uses library.") |
      Opt(port, "2030")["-p"]["--port"]("Specify port for service to connect to (default 2030)") |
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
    std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
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

  if ( server.empty() )
  {
    auto encrypter = Encrypter( Encrypter::AES_256_CBC, key() );
    if ( encrypt )
    {
      const auto encrypted = encrypter.encrypt( input );
      std::cout << encrypted << std::endl;
    }

    if ( decrypt )
    {
      const auto dec = encrypter.decrypt( input );
      std::cout << dec << std::endl;
    }

    return 0;
  }

  LOG_INFO << "Using service at " << server << ':' << port;
  try
  {
    auto c = spt::encrypter::api::tcp::Connection{
      spt::encrypter::api::ContextHolder::instance().ioc, server, port };

    if ( encrypt )
    {
      const auto encrypted = c.encrypt( input );
      std::cout << encrypted << std::endl;
    }

    if ( decrypt )
    {
      const auto dec = c.decrypt( input );
      std::cout << dec << std::endl;
    }
  }
  catch ( const std::exception& ex )
  {
    LOG_WARN << "Error connecting to encrypter service. " << ex.what();
    exit( 3 );
  }

  return 0;
}