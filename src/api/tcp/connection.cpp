//
// Created by Rakesh on 15/01/2022.
//

#include "../api.h"
#include "../contextholder.h"
#include "connection.h"
#include "../../log/NanoLog.h"

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
}

void spt::encrypter::api::init( std::string_view server, std::string_view port )
{
  if ( tcp::pconnection::server.empty() )
  {
    auto lock = std::unique_lock( tcp::pconnection::mutex );
    tcp::pconnection::server = std::string{ server.data(), server.size() };
    tcp::pconnection::port = std::string{ port.data(), port.size() };
  }
  else
  {
    LOG_CRIT << "Logic error.  Attempting to initialise API again.";
  }
}

Connection::Connection( boost::asio::io_context& ioc, std::string_view server, std::string_view port ) :
    s{ ioc }, resolver{ ioc }
{
  boost::asio::connect( s, resolver.resolve( server, port ) );
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
  os.write( reinterpret_cast<const char*>( &n ), sizeof(n) );
  os.write( data.data(), data.size() );

  const auto isize = boost::asio::write( s, buffer );
  buffer.consume( isize );

  auto osize = s.read_some( buffer.prepare( data.size() ) );
  buffer.commit( osize );
  std::size_t read = osize;

  if ( read < 5 )
  {
    LOG_WARN << "Invalid short response for " << context;
    return {};
  }

  const auto d = reinterpret_cast<const uint8_t*>( buffer.data().data() );
  uint32_t len;
  memcpy( &len, d, sizeof(len) );
  LOG_DEBUG << "Read " << int(read) << " bytes, total size " << int(len);

  auto i = 0;
  while ( read < ( len + sizeof(len) ) )
  {
    LOG_DEBUG << "Iteration " << ++i;
    osize = s.read_some( buffer.prepare( 256 ) );
    buffer.commit( osize );
    read += osize;
  }

  LOG_DEBUG << "Read " << int(read) << " bytes, total size " << int(len);
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
  return std::make_unique<Connection>( ContextHolder::instance().ioc, pconnection::server, pconnection::port );
}