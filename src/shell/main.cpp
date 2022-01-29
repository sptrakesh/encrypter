//
// Created by Rakesh on 25/12/2021.
//

#include "../api/api.h"
#include "../lib/util/clara.h"
#include "../lib/util/encrypter.h"
#include "../log/NanoLog.h"

#include <iostream>

#include <readline/readline.h>
#include <readline/history.h>

namespace spt::encrypter::shell
{
  std::string key()
  {
    char* val = std::getenv( "ENCRYPTION_KEY" );
    return val == nullptr ? std::string( "rFmHPdwX5E/2gq2fSuPrgSDUi4W/+Y7/QL3NxINM+pY=" ) : std::string( val );
  }

  void help()
  {
    std::cout << "\033[1mAvailable commands\033[0m" << '\n';
    std::cout << "  \033[1menc\033[0m \033[3m<value>\033[0m - Encrypt the value.  Eg. [enc some words with . and \"quotes\"]" << '\n';
    std::cout << "  \033[1mdec\033[0m \033[3m<encrypted value>\033[0m - Decrypt the value.  Eg. [dec haxRkxWhpzHeWnnlynSEqq9Lvvs4pJ3BkfAWU0VXwbc=]" << '\n';
  }

  std::string_view trim( std::string_view in )
  {
    auto left = in.begin();
    for ( ;; ++left )
    {
      if ( left == in.end() ) return in;
      if ( !std::isspace( *left ) ) break;
    }
    auto right = in.end() - 1;
    for ( ; right > left && std::isspace( *right ); --right );
    return { left, static_cast<std::size_t>(std::distance( left, right ) + 1) };
  }

  std::tuple<std::string_view, std::size_t> command( std::string_view line )
  {
    auto idx = line.find( ' ', 0 );
    if ( idx == std::string_view::npos ) return { line, idx };
    return { line.substr( 0, idx ), idx };
  }

  std::tuple<std::string_view, std::size_t> key( std::string_view line, std::size_t begin )
  {
    auto idx = line.find( ' ', begin + 1 );
    while ( idx != std::string::npos && line.substr( begin + 1, idx - begin - 1 ).empty() )
    {
      ++begin;
      idx = line.find( ' ', begin + 1 );
    }

    return { line.substr( begin + 1 ), idx };
  }

  void encrypt( util::Encrypter& encrypter, std::string_view line, std::size_t idx )
  {
    auto [k, end] = key( line, idx );
    if ( end <= idx )
    {
      std::cout << "Cannot parse text from " << line << '\n';
      return;
    }

    auto res = encrypter.encrypt( k );
    if ( res.empty() )
    {
      std::cout << "Cannot encrypt text " << k << '\n';
      return;
    }
    std::cout << res << '\n';
  }

  void encrypt( std::string_view line, std::size_t idx )
  {
    auto [k, end] = key( line, idx );
    if ( end <= idx )
    {
      std::cout << "Cannot parse text from " << line << '\n';
      return;
    }

    auto res = api::encrypt( k );
    if ( res.empty() )
    {
      std::cout << "Cannot encrypt text " << k << '\n';
      return;
    }
    std::cout << res << '\n';
  }

  void decrypt( util::Encrypter& encrypter, std::string_view line, std::size_t idx )
  {
    auto [k, end] = key( line, idx );
    if ( end <= idx )
    {
      std::cout << "Cannot parse text from " << line << '\n';
      return;
    }

    auto res = encrypter.decrypt( k );
    if ( res.empty() )
    {
      std::cout << "Cannot decrypt text " << k << '\n';
      return;
    }
    std::cout << res << '\n';
  }

  void decrypt( std::string_view line, std::size_t idx )
  {
    auto [k, end] = key( line, idx );
    if ( end <= idx )
    {
      std::cout << "Cannot parse text from " << line << '\n';
      return;
    }

    auto res = api::decrypt( k );
    if ( res.empty() )
    {
      std::cout << "Cannot decrypt text " << k << '\n';
      return;
    }
    std::cout << res << '\n';
  }

  void run()
  {
    using namespace std::literals;
    std::cout << "Enter commands followed by <ENTER>" << '\n';
    std::cout << "Enter \033[1mhelp\033[0m for help about commands" << '\n';
    std::cout << "Enter \033[1mexit\033[0m or \033[1mquit\033[0m to exit shell\n";

    // Disable tab completion
    rl_bind_key( '\t', rl_insert );

    auto encrypter = util::Encrypter( util::Encrypter::AES_256_CBC, key() );

    std::string previous;
    previous.reserve( 128 );

    char* buf;
    while ( ( buf = readline("encrypter> " ) ) != nullptr )
    {
      auto len = strlen( buf );
      if ( len == 0 )
      {
        std::free( buf );
        continue;
      }

      if ( previous != std::string{ buf } ) add_history( buf );

      auto line = std::string_view{ buf, len };
      line = trim( line );
      if ( line == "exit"sv || line == "quit"sv )
      {
        std::cout << "Bye\n";
        break;
      }
      else if ( line == "help"sv ) help();
      else if ( line.empty() ) { /* noop */ }
      else
      {
        auto[cmd, idx] = command( line );

        if ( "enc"sv == cmd )
        {
          encrypt( encrypter, line, idx );
        }
        else if ( "dec"sv == cmd )
        {
          decrypt( encrypter, line, idx );
        }
        else
        {
          std::cout << "Unknown command " << cmd << '\n';
        }
      }

      previous.clear();
      previous.append( buf, len );
      std::free( buf );
    }
  }

  void run( std::string_view server, std::string_view port )
  {
    api::init( server, port );

    using namespace std::literals;
    std::cout << "Using encrypter service on \033[1m" << server << ':' << port << "\033[1m\n";
    std::cout << "Enter commands followed by <ENTER>" << '\n';
    std::cout << "Enter \033[1mhelp\033[0m for help about commands" << '\n';
    std::cout << "Enter \033[1mexit\033[0m or \033[1mquit\033[0m to exit shell\n";

    // Disable tab completion
    rl_bind_key( '\t', rl_insert );

    std::string previous;
    previous.reserve( 128 );

    char* buf;
    while ( ( buf = readline("encrypter> " ) ) != nullptr )
    {
      auto len = strlen( buf );
      if ( len == 0 )
      {
        std::free( buf );
        continue;
      }

      if ( previous != std::string{ buf } ) add_history( buf );

      auto line = std::string_view{ buf, len };
      line = trim( line );
      if ( line == "exit"sv || line == "quit"sv )
      {
        std::cout << "Bye\n";
        break;
      }
      else if ( line == "help"sv ) help();
      else if ( line.empty() ) { /* noop */ }
      else
      {
        auto[cmd, idx] = command( line );

        if ( "enc"sv == cmd )
        {
          encrypt( line, idx );
        }
        else if ( "dec"sv == cmd )
        {
          decrypt( line, idx );
        }
        else
        {
          std::cout << "Unknown command " << cmd << '\n';
        }
      }

      previous.clear();
      previous.append( buf, len );
      std::free( buf );
    }
  }
}

int main( int argc, char const * const * argv )
{
  using clara::Opt;
  std::string server;
  std::string port{ "2030" };
#ifdef __APPLE__
  std::string logLevel{ "debug" };
#else
  std::string logLevel{"info"};
#endif
  std::string dir{"/tmp/"};
  bool help = false;

  auto options = clara::Help(help) |
      Opt(server, "localhost")["-s"]["--server"]("Specify service to connect to.  If not specified uses library.") |
      Opt(port, "2030")["-p"]["--port"]("Specify port for service to connect to (default 2030)") |
      Opt(logLevel, "info")["-l"]["--log-level"]("Log level to use [debug|info|warn|critical] (default info).") |
      Opt(dir, "/tmp/")["-o"]["--log-dir"]("Log directory (default /tmp/)");

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

  if ( logLevel == "debug" ) nanolog::set_log_level( nanolog::LogLevel::DEBUG );
  else if ( logLevel == "info" ) nanolog::set_log_level( nanolog::LogLevel::INFO );
  else if ( logLevel == "warn" ) nanolog::set_log_level( nanolog::LogLevel::WARN );
  else if ( logLevel == "critical" ) nanolog::set_log_level( nanolog::LogLevel::CRIT );
  nanolog::initialize( nanolog::GuaranteedLogger(), dir, "encrypter-shell", false );

  if ( server.empty() ) spt::encrypter::shell::run();
  else spt::encrypter::shell::run( server, port );
}
