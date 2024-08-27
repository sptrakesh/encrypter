//
// Created by Rakesh on 15/01/2022.
//

#pragma once

#include <memory>
#include <string>
#include <string_view>

#include <boost/asio/io_context.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace spt::encrypter::api::tcp
{
  struct Connection
  {
    Connection( boost::asio::io_context& ioc, std::string_view server, std::string_view port );
    ~Connection();

    std::string encrypt( std::string_view data );
    std::string decrypt( std::string_view data );

    bool valid() const { return state; }
    void invalid() { state = false; }

  private:
    std::string send( std::string_view data, std::string_view context );
    void socket();

    boost::asio::ip::tcp::socket s;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::streambuf buffer;
    bool state{ true };
  };

  std::unique_ptr<Connection> create();
}