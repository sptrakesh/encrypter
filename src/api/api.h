//
// Created by Rakesh on 15/01/2022.
//

#pragma once

#include <string>
#include <string_view>
#include "contextholder.h"

namespace spt::encrypter::api
{
  /**
   * Initialise the client with the service running on the specified host and port.
   *
   * **Note:** Must be invoked before attempting to use the API functions.
   *
   * @param server The hostname of the service to connect to.
   * @param port The port to connect to.
   * @param ioc The optional IO context to use.  Defaults to the context from {@see ContextHolder::ioc}
   */
  void init( std::string_view server, std::string_view port = { "2030" },
      boost::asio::io_context& ioc = ContextHolder::instance().ioc );

  /**
   * Encrypt the specified data.  If errors are encountered while encrypting
   * an empty string is returned.
   *
   * @param data The data to encrypt.
   * @return The encrypted value
   */
  std::string encrypt( std::string_view data );

  /**
   * Decrypt the specified data.  If errors are encountered while decrypting
   * an empty string is returned.
   *
   * @param data The data to decrypt.  Must have been generated by a prior invocation
   *   of encrypt or its equivalent (via utility app etc)
   * @return The decrypted value.
   */
  std::string decrypt( std::string_view data );
}