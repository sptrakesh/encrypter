//
// Created by Rakesh on 15/01/2022.
//

#include "api.hpp"
#include "tcp/connection.hpp"
#include "../lib/pool/pool.hpp"

namespace spt::encrypter::api::ppool
{
  struct PoolHolder
  {
    static PoolHolder& instance()
    {
      static PoolHolder p;
      return p;
    }

    ~PoolHolder() = default;
    PoolHolder(const PoolHolder&) = delete;
    PoolHolder& operator=(const PoolHolder&) = delete;

    auto acquire() { return pool.acquire(); }

  private:
    PoolHolder() = default;

    spt::encrypter::pool::Pool<tcp::Connection> pool{ spt::encrypter::api::tcp::create, pool::Configuration{} };
  };
}

std::string spt::encrypter::api::encrypt( std::string_view data )
{
  auto popt = ppool::PoolHolder::instance().acquire();
  if ( !popt )
  {
    LOG_CRIT << "Error acquiring connection from pool";
    return {};
  }

  return (*popt)->encrypt( data );
}

std::string spt::encrypter::api::decrypt( std::string_view data )
{
  auto popt = ppool::PoolHolder::instance().acquire();
  if ( !popt )
  {
    LOG_CRIT << "Error acquiring connection from pool";
    return {};
  }

  return (*popt)->decrypt( data );
}

