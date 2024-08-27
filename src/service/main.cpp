//
// Created by Rakesh on 13/01/2022.
//

#include <iostream>
#include <thread>

#include "service.hpp"
#include "../lib/util/clara.hpp"
#include "../log/NanoLog.hpp"

int main( int argc, char const * const * argv )
{
  using clara::Opt;

  std::string level{ "info" };
  std::string dir{ "/tmp/" };
  int port{ 2030 };
  auto threads = std::thread::hardware_concurrency();
  bool console = false;
  bool help = false;

  auto options = clara::Help(help) |
      Opt(console)["-c"]["--console"]("Log to console (default off)") |
      Opt(port, "2030")["-p"]["--port"]("Port on which to listen for tcp traffic (default 2030)") |
      Opt(threads, "8")["-n"]["--threads"]("Number of server threads to spawn (default system)") |
      Opt(level, "info")["-l"]["--log-level"]("Log level to use [debug|info|warn|critical] (default info).") |
      Opt(dir, "logs/")["-o"]["--log-dir"]("Log directory (default logs/)");

  auto result = options.parse( clara::Args( argc, argv ) );
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

  if ( level == "debug" ) nanolog::set_log_level( nanolog::LogLevel::DEBUG );
  else if ( level == "info" ) nanolog::set_log_level( nanolog::LogLevel::INFO );
  else if ( level == "warn" ) nanolog::set_log_level( nanolog::LogLevel::WARN );
  else if ( level == "critical" ) nanolog::set_log_level( nanolog::LogLevel::CRIT );
  nanolog::initialize( nanolog::GuaranteedLogger(), dir, "encrypter-service", console );

  return spt::encrypter::service::run( port, threads );
}

