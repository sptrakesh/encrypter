#include "encrypter.h"
#include "base64.h"
#include "../log/NanoLog.h"

#include <chrono>

#include <openssl/err.h>

using spt::util::Encrypter;

Encrypter::Encrypter( std::string_view encScheme, std::string_view cryptKey )
    : scheme( encScheme ), key( cryptKey )
{
  auto start = std::chrono::high_resolution_clock::now();
  loadOpenSSL();
  initContext();
  auto finish = std::chrono::high_resolution_clock::now();
  LOG_INFO << "Initialising OpenSSL routines took " <<
    std::chrono::duration_cast<std::chrono::nanoseconds>( finish - start ).count() <<
    " nanoseconds";
}

Encrypter::~Encrypter()
{
  cleanOpenSSL();
}

std::string Encrypter::encrypt( std::string_view value )
{
  auto const execute = [this, value]() -> std::string
  {
    auto outlen = static_cast<int>( value.size() );
    std::string str( outlen + EVP_CIPHER_CTX_block_size( encryptingContext ), '\0' );
    auto* outbuf = reinterpret_cast<unsigned char*>( str.data() );

    if ( refreshEncryptContext )
    {
      EVP_EncryptInit_ex( encryptingContext, nullptr, nullptr, nullptr, nullptr );
    }
    else
    {
      refreshEncryptContext = true;
    }

    if ( ! EVP_EncryptUpdate( encryptingContext, outbuf, &outlen,
        reinterpret_cast<const unsigned char *>( value.data() ), outlen ) )
    {
      LOG_WARN << "EVP_EncryptUpdate() failed!";
      printError();
      return {};
    }

    int templen;
    if ( ! EVP_EncryptFinal_ex( encryptingContext, outbuf + outlen, &templen ) )
    {
      LOG_WARN << "EVP_EncryptFinal() failed!";
      printError();
      return {};
    }

    outlen += templen;
    return base64_encode( outbuf, outlen );
  };

  if ( value.empty() ) return {};

  auto start = std::chrono::high_resolution_clock::now();
  auto result = execute();
  auto finish = std::chrono::high_resolution_clock::now();
  LOG_DEBUG << "Encryption of value of length " << int( value.size() ) <<
    " took " << std::chrono::duration_cast<std::chrono::nanoseconds>( finish - start ).count() <<
    " nanoseconds";
  return result;
}

std::string Encrypter::decrypt( std::string_view sec )
{
  if ( sec.empty() ) return {};

  auto const execute = [this]( std::string_view source ) -> std::string
  {
    auto sec = base64_decode( source );
    auto outlen = static_cast<int>( sec.size() );
    std::string str( outlen, '\0' );
    auto* outbuf = reinterpret_cast<unsigned char *>( str.data() );

    if ( refreshDecryptContext )
    {
      EVP_DecryptInit_ex( decryptingContext, nullptr, nullptr, nullptr, nullptr );
    }
    else
    {
      refreshDecryptContext = true;
    }

    auto* input = reinterpret_cast<const unsigned char*>( sec.data() );
    if ( ! EVP_DecryptUpdate( decryptingContext, outbuf, &outlen, input, outlen ) )
    {
      LOG_WARN << "EVP_DecryptUpdate() failed!";
      printError();
      return {};
    }

    int templen;
    if ( ! EVP_DecryptFinal_ex( decryptingContext, outbuf + outlen, &templen ) )
    {
      LOG_WARN << "EVP_DecryptFinal() failed!";
      printError();
      return {};
    }

    outlen += templen;
    return std::string{ reinterpret_cast<char*>( outbuf ), static_cast<std::size_t>( outlen ) };
  };

  auto start = std::chrono::high_resolution_clock::now();
  auto result = execute( sec );
  auto finish = std::chrono::high_resolution_clock::now();
  LOG_DEBUG << "Decryption of value of length " << int( sec.size() ) <<
    " took " << std::chrono::duration_cast<std::chrono::nanoseconds>( finish - start ).count() <<
    " nanoseconds";
  return result;
}

void Encrypter::loadOpenSSL()
{
  OpenSSL_add_all_algorithms();
  ERR_load_crypto_strings();
}

void Encrypter::initContext()
{
  int rounds = 1000000;

  static const std::string saltStr = "xpUYAiWFIXx93GZgDlnRmkJo1Sn7LGP";
  auto* salt = reinterpret_cast<const unsigned char*>( saltStr.data() );

  std::string keystr = "OpJ8onxxqTkkB6xfaU0fqq+5WAm4Jxp";
  auto* aes_key = reinterpret_cast<unsigned char*>( keystr.data() );

  std::string ivstr = "ZblGB8ednebk3Axk3n48v5fVzSgHy/D";
  auto* aes_iv = reinterpret_cast<unsigned char*>( ivstr.data() );

  const EVP_CIPHER *cipher = EVP_get_cipherbyname( scheme.c_str() );
  if ( ! cipher )
  {
    LOG_WARN << "Cannot get cipher with name {" << scheme << '}';
    printError();
    throw SSLException{};
  }

  EVP_BytesToKey(
             cipher,  // Cryptographic mode
             EVP_sha1(),         // SHA1
             salt,               // a fuzzifier
             reinterpret_cast<const unsigned char *>( key.c_str() ),
             key.size() + 1,
             rounds,             // more rounds
             aes_key, aes_iv);   // return buffers

  encryptingContext = EVP_CIPHER_CTX_new();
  EVP_CIPHER_CTX_init( encryptingContext );

  EVP_EncryptInit_ex( encryptingContext, cipher, nullptr, aes_key, aes_iv );

  decryptingContext = EVP_CIPHER_CTX_new();
  EVP_CIPHER_CTX_init( decryptingContext );
  EVP_DecryptInit_ex( decryptingContext, cipher, nullptr, aes_key, aes_iv );
}

void Encrypter::cleanOpenSSL()
{
  EVP_CIPHER_CTX_free( encryptingContext );
  EVP_CIPHER_CTX_free( decryptingContext );
  EVP_cleanup();
  ERR_free_strings();
}

void Encrypter::printError()
{
  unsigned long errorCode;
  while( ( errorCode = ERR_get_error() ) > 0 )
  {
    char *errorMessage = ERR_error_string( errorCode, nullptr );
    LOG_WARN << "Error code: {" << int(errorCode) << "} message: {" << errorMessage << "}";
  }
}
