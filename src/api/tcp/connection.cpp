//
// Created by Rakesh on 15/01/2022.
//

#include "../api.hpp"
#include "connection.hpp"
#include "../../log/NanoLog.hpp"

#include <mutex>
#include <boost/asio/connect.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

using spt::encrypter::api::tcp::Connection;
using namespace std::string_view_literals;

namespace spt::encrypter::api::tcp::pconnection
{
  std::mutex mutex;
  std::string server;
  std::string port;
  boost::asio::io_context* ioc{ nullptr };
}

void spt::encrypter::api::init( std::string_view server, std::string_view port,
    boost::asio::io_context& ioc )
{
  if ( tcp::pconnection::server.empty() )
  {
    auto lock = std::unique_lock( tcp::pconnection::mutex );
    tcp::pconnection::server.append( server.data(), server.size() );
    tcp::pconnection::port.append( port.data(), port.size() );
    tcp::pconnection::ioc = &ioc;
    LOG_INFO << "Initialised client connection to " << tcp::pconnection::server << ':' << tcp::pconnection::port;
  }
  else
  {
    LOG_CRIT << "Logic error.  Attempting to initialise API again.";
  }
}

Connection::Connection( boost::asio::io_context& ioc, std::string_view server, std::string_view port ) :
    s{ ioc }, resolver{ ioc }
{
  boost::system::error_code ec;
  auto servers = resolver.resolve( server, port, ec );
  if ( ec )
  {
    LOG_CRIT << "Error resolving encrypter service " << server << ':' << port;
    throw std::runtime_error( "Error resolving encrypter service" );
  }
  boost::asio::connect( s, servers, ec );
  if ( ec )
  {
    LOG_CRIT << "Error connecting to encrypter service " << server << ':' << port;
    throw std::runtime_error( "Error connecting to encrypter service" );
  }
}

Connection::~Connection()
{
  if ( s.is_open() )
  {
    boost::system::error_code ec;
    s.close( ec );
    if ( ec ) LOG_CRIT << "Error closing socket. " << ec.message();
  }
}

std::string Connection::encrypt( std::string_view data )
{
  std::string req;
  req.reserve( data.size() + 2 );
  req.append( "e " );
  req.append( data.data(), data.size() );
  return send( req, "encrypt"sv );
}

std::string Connection::decrypt( std::string_view data )
{
  std::string req;
  req.reserve( data.size() + 2 );
  req.append( "d " );
  req.append( data.data(), data.size() );
  return send( req, "decrypt"sv );
}

std::string Connection::send( std::string_view data, std::string_view context )
{
  socket();
  if ( !state ) return {};

  uint32_t n = data.size();
  std::ostream os{ &buffer };
  os.write( reinterpret_cast<const char*>( &n ), sizeof( n ) );
  os.write( data.data(), data.size() );

  const auto isize = boost::asio::write( s, buffer );
  buffer.consume( isize );

  auto osize = s.read_some( buffer.prepare( data.size() ) );
  buffer.commit( osize );
  std::size_t read = osize;

  if ( read < 5 ) // flawfinder: ignore
  {
    LOG_WARN << "Invalid short response for " << context;
    return {};
  }

  const auto d = reinterpret_cast<const uint8_t*>( buffer.data().data() );
  uint32_t len;
  memcpy( &len, d, sizeof( len ) );
  LOG_DEBUG << "Read " << int( read ) << " bytes, total size " << int( len ); // flawfinder: ignore

#ifndef MAX_REQUEST_SIZE
#define MAX_REQUEST_SIZE (256*1024)
#endif

  if ( uint32_t maxLength = std::max( len, uint32_t(MAX_REQUEST_SIZE) ); len > maxLength )
  {
    LOG_WARN << "Request size exceeds max defined size " << int(MAX_REQUEST_SIZE) << ".  Truncating read...";
    len = maxLength;
  }

  auto i = 0;
  while ( read < ( len + sizeof(len) ) ) // flawfinder: ignore
  {
    LOG_DEBUG << "Iteration " << ++i;
    osize = s.read_some( buffer.prepare( 256 ) );
    buffer.commit( osize );
    read += osize;
  }

  LOG_DEBUG << "Read " << int(read) << " bytes, total size " << int(len); // flawfinder: ignore
  auto resp = std::string{ reinterpret_cast<const char*>( buffer.data().data()  ) + sizeof(len), len };
  buffer.consume( buffer.size() );
  return resp;
}

void Connection::socket()
{
  if ( s.is_open() ) return;

  LOG_DEBUG << "Re-opening closed connection.";
  boost::system::error_code ec;
  boost::asio::connect( s, resolver.resolve( pconnection::server, pconnection::port ), ec );
  if ( ec )
  {
    LOG_WARN << "Error opening socket connection. " << ec.message();
    invalid();
    return;
  }
  boost::asio::socket_base::keep_alive option( true );
  s.set_option( option );
}

std::unique_ptr<Connection> spt::encrypter::api::tcp::create()
{
  return std::make_unique<Connection>( *pconnection::ioc, pconnection::server, pconnection::port );
}