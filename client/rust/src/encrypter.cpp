//
// Created by Rakesh on 15/10/2025.
//

#include "encrypter/include/encrypter.hpp"

#include <string_view>
#include <encrypter/api/api.hpp>
#include <log/NanoLog.hpp>

void init( const Configuration& conf )
{
  switch ( conf.log_level )
  {
    case LogLevel::DEBUG: nanolog::set_log_level( nanolog::LogLevel::DEBUG ); break;
    case LogLevel::INFO: nanolog::set_log_level( nanolog::LogLevel::INFO ); break;
    case LogLevel::WARN: nanolog::set_log_level( nanolog::LogLevel::WARN ); break;
    case LogLevel::CRIT: nanolog::set_log_level( nanolog::LogLevel::CRIT ); break;
  }
  nanolog::initialize( nanolog::GuaranteedLogger(), "/tmp/", "encrypter-rust", conf.log_to_console );
  auto sh = std::string_view{ conf.host.begin(), conf.host.end() };
  auto sp = std::to_string( conf.port );
  spt::encrypter::api::init( sh, sp );
}

rust::String encrypt( rust::Str data )
{
  auto str = std::string_view{ data.begin(), data.end() };
  return spt::encrypter::api::encrypt( str );
}

rust::String decrypt( rust::Str data )
{
  auto str = std::string_view{ data.begin(), data.end() };
  return spt::encrypter::api::decrypt( str );
}
