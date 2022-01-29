//
// Created by Rakesh on 13/01/2022.
//

#include "service.h"
#include "../lib/pool/pool.h"
#include "../lib/util/encrypter.h"

#include <thread>
#include <vector>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/redirect_error.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace spt::encrypter::service::internal
{
  std::unique_ptr<util::Encrypter> create()
  {
    return std::make_unique<util::Encrypter>( util::Encrypter::AES_256_CBC, "rFmHPdwX5E/2gq2fSuPrgSDUi4W/+Y7/QL3NxINM+pY=" );
  }

  spt::encrypter::pool::Configuration poolConfig()
  {
    auto config = spt::encrypter::pool::Configuration{};
    config.initialSize = 2;
    config.maxPoolSize = std::numeric_limits<int>::max();
    config.maxConnections = std::numeric_limits<int>::max();
    config.maxIdleTime = std::chrono::days{ std::numeric_limits<int>::max() };
    return config;
  }

  struct PoolHolder
  {
    static PoolHolder& instance()
    {
      static PoolHolder h;
      return h;
    }

    ~PoolHolder() = default;
    PoolHolder(const PoolHolder&) = delete;
    PoolHolder& operator=(const PoolHolder&) = delete;

    auto acquire() { return pool.acquire(); }

  private:
    PoolHolder() = default;
    spt::encrypter::pool::Pool<util::Encrypter> pool{ spt::encrypter::service::internal::create, poolConfig() };
  };
}

namespace spt::encrypter::service::coroutine
{
  std::string encrypt( std::string_view data )
  {
    auto encrypter = internal::PoolHolder::instance().acquire();
    if ( !encrypter )
    {
      LOG_CRIT << "Unable to acquire encrypter from pool";
      return {};
    }
    return ( *encrypter )->encrypt( data );
  }

  std::string decrypt( std::string_view data )
  {
    auto encrypter = internal::PoolHolder::instance().acquire();
    if ( !encrypter )
    {
      LOG_CRIT << "Unable to acquire encrypter from pool";
      return {};
    }
    return ( *encrypter )->decrypt( data );
  }

  boost::asio::awaitable<void> write( boost::asio::ip::tcp::socket& socket, std::string_view data )
  {
    uint32_t size = data.size();
    std::vector<boost::asio::const_buffer> buffers;
    buffers.reserve( 2 );
    buffers.emplace_back( &size, sizeof(size) );
    buffers.emplace_back( data.data(), size );
    boost::system::error_code ec;
    co_await boost::asio::async_write( socket, buffers,
        boost::asio::redirect_error( boost::asio::use_awaitable, ec ) );
    if ( ec ) LOG_WARN << "Error writing to socket. " << ec.message();
    else LOG_DEBUG << "Finished writing data of size " << int(size) << " + " << int(sizeof(size)) << " bytes";
  }

  bool brokenPipe( const boost::system::error_code& ec )
  {
    static const std::string msg{ "Broken pipe" };
    return boost::algorithm::starts_with( ec.message(), msg );
  }

  boost::asio::awaitable<void> process( boost::asio::ip::tcp::socket& socket, std::string_view sv )
  {
    using namespace std::string_view_literals;

    if ( sv.starts_with( "e "sv ) )
    {
      auto result = encrypt( sv.substr( 2 ) );
      co_await write( socket, result );
    }
    else if ( sv.starts_with( "d "sv ) )
    {
      auto result = decrypt( sv.substr( 2 ) );
      co_await write( socket, result );
    }
    else
    {
      LOG_WARN << "Invalid request " << sv;
      boost::system::error_code ec;
      co_await boost::asio::async_write( socket, boost::asio::buffer( sv.data(), sv.size() ),
          boost::asio::redirect_error( boost::asio::use_awaitable, ec ) );
      if ( ec && !brokenPipe( ec ) )
      {
        LOG_WARN << "Error writing to socket. " << ec.message();
      }
    }
  }

  boost::asio::awaitable<void> respond( boost::asio::ip::tcp::socket& socket )
  {
    static constexpr int bufSize = 128;
    static constexpr auto maxBytes = 8 * 1024 * 1024;
    uint8_t data[bufSize];

    const auto documentSize = [&data]( std::size_t length )
    {
      if ( length < 5 ) return length;

      const auto d = const_cast<uint8_t*>( data );
      uint32_t len;
      memcpy( &len, d, sizeof(len) );
      return std::size_t( len + sizeof(len) );
    };

    boost::system::error_code ec;
    std::size_t osize = co_await socket.async_read_some( boost::asio::buffer( data ), boost::asio::use_awaitable );
    const auto docSize = documentSize( osize );

    // echo, noop, ping etc.
    if ( docSize < 5 )
    {
      co_await boost::asio::async_write( socket, boost::asio::buffer( data, docSize ),
          boost::asio::redirect_error( boost::asio::use_awaitable, ec ) );
      if ( ec && !brokenPipe( ec ) )
      {
        LOG_WARN << "Error writing to socket. " << ec.message();
      }
      co_return;
    }

    if ( docSize <= bufSize )
    {
      auto d = reinterpret_cast<const char*>( data );
      auto size = sizeof(uint32_t);
      auto sv = std::string_view{ d + size, docSize - size };
      co_await process( socket, sv );
      co_return;
    }

    auto read = osize;
    std::vector<uint8_t> rbuf;
    rbuf.reserve( docSize - sizeof(uint32_t) );
    rbuf.insert( rbuf.end(), data + sizeof(uint32_t), data + osize );

    LOG_DEBUG << "Read " << int(osize) << " bytes, total size " << int(docSize);
    while ( docSize < maxBytes && read != docSize )
    {
      osize = co_await socket.async_read_some( boost::asio::buffer( data ), boost::asio::use_awaitable );
      rbuf.insert( rbuf.end(), data, data + osize );
      read += osize;
    }

    auto sv = std::string_view{ reinterpret_cast<const char*>( rbuf.data() ), rbuf.size() };
    co_await process( socket, sv );
  }

  boost::asio::awaitable<void> serve( boost::asio::ip::tcp::socket socket )
  {
    try
    {
      for (;;)
      {
        co_await respond( socket );
      }
    }
    catch ( const std::exception& e )
    {
      static const auto eof{ "End of file" };
      if ( !boost::algorithm::starts_with( e.what(), eof ) ) LOG_WARN << "Exception servicing request " << e.what();
    }
  }

  boost::asio::awaitable<void> listener( boost::asio::io_context& ioc, int port )
  {
    auto executor = co_await boost::asio::this_coro::executor;
    boost::asio::ip::tcp::acceptor acceptor( executor,
        { boost::asio::ip::tcp::v4(), static_cast<boost::asio::ip::port_type>( port ) } );
    for (;;)
    {
      if ( ioc.stopped() ) break;
      boost::asio::ip::tcp::socket socket = co_await acceptor.async_accept( boost::asio::use_awaitable );
      boost::asio::co_spawn( executor, serve( std::move(socket) ), boost::asio::detached );
    }
  }
}

int spt::encrypter::service::run( int port, int threads )
{
  boost::asio::io_context ioc{ threads };

  boost::asio::signal_set signals( ioc, SIGINT, SIGTERM );
  signals.async_wait( [&ioc](auto const&, int ) { ioc.stop(); } );

  const auto run = [&ioc]
  {
    for (;;)
    {
      try
      {
        ioc.run();
        break;
      }
      catch ( std::exception& e )
      {
        LOG_CRIT << "Unhandled exception " << e.what();
        ioc.run();
      }
    }
  };

  std::vector<std::thread> v;
  for( auto i = threads; i > 0; --i )
  {
    v.emplace_back( [&run] { run(); } );
  }

  internal::PoolHolder::instance();
  boost::asio::co_spawn( ioc, coroutine::listener( ioc, port ), boost::asio::detached );
  LOG_INFO << "TCP service started on port " << port;

  run();

  LOG_INFO << "TCP service stopped";
  for ( auto&& t : v ) if ( t.joinable() ) t.join();

  return 0;
}

