#ifndef ENCRYPTER_H
#define ENCRYPTER_H

#include <string>
#include <string_view>

#include <openssl/evp.h>

namespace spt::util
{
  /**
   * A general purpose encrypter/decrypter using OpenSSL.  Initialise
   * with a specified encryption routine (use the constants defined
   * for convenience).  Primarily designed to encrypt/decrypt using
   * a predefined secret.  If you wish to use custom keys to encrypt
   * decrypt a specified value, then you will need to create different
   * instances of the encrypter for use with different key values.
   */
  struct Encrypter
  {
    struct SSLException : std::exception
    {
      [[nodiscard]] const char* what() const noexcept override
      {
        return "Error initialising OpenSSL";
      }
    };

    /**
     * Constants used to look up the encryption scheme.
     * See http://mspmsp.brinkster.net/SPC/0596003943_secureprgckbk-chp-5-sect-17.html
     */
    static inline const std::string AES_128_ECB = "aes-128-ecb";
    static inline const std::string AES_128_CBC = "aes-128-cbc";
    static inline const std::string AES_128_CFB = "aes-128-cfb";
    static inline const std::string AES_128_OFB = "aes-128-ofb";

    static inline const std::string AES_192_ECB = "aes-192-ecb";
    static inline const std::string AES_192_CBC = "aes-192-cbc";
    static inline const std::string AES_192_CFB = "aes-192-cfb";
    static inline const std::string AES_192_OFB = "aes-192-ofb";

    static inline const std::string AES_256_ECB = "aes-256-ecb";
    static inline const std::string AES_256_CBC = "aes-256-cbc";
    static inline const std::string AES_256_CFB = "aes-256-cfb";
    static inline const std::string AES_256_OFB = "aes-256-ofb";

    static inline const std::string BLOWFISH_ECB = "bf-ecb";
    static inline const std::string BLOWFISH_CBC = "bf-cbc";
    static inline const std::string BLOWFISH_CFB = "bf-cfb";
    static inline const std::string BLOWFISH_OFB = "bf-ofb";

    static inline const std::string CAST5_ECB = "cast-ecb";
    static inline const std::string CAST5_CBC = "cast-cbc";
    static inline const std::string CAST5_CFB = "cast-cfb";
    static inline const std::string CAST5_OFB = "cast-ofb";

    static inline const std::string DES_ECB = "des-ecb";
    static inline const std::string DES_CBC = "des-cbc";
    static inline const std::string DES_CFB = "des-cfb";
    static inline const std::string DES_OFB = "des-ofb";

    static inline const std::string DES_EDE_ECB = "des-ede-ecb";
    static inline const std::string DES_EDE_CBC = "des-ede-cbc";
    static inline const std::string DES_EDE_CFB = "des-ede-cfb";
    static inline const std::string DES_EDE_OFB = "des-ede-ofb";

    static inline const std::string DES_EDE3_ECB = "des-ede3-ecb";
    static inline const std::string DES_EDE3_CBC = "des-ede3-cbc";
    static inline const std::string DES_EDE3_CFB = "des-ede3-cfb";
    static inline const std::string DES_EDE3_OFB = "des-ede3-ofb";

    static inline const std::string IDEA_ECB = "idea-ecb";
    static inline const std::string IDEA_CBC = "idea-cbc";
    static inline const std::string IDEA_CFB = "idea-cfb";
    static inline const std::string IDEA_OFB = "idea-ofb";

    static inline const std::string RC2_ECB = "rc2-ecb";
    static inline const std::string RC2_CBC = "rc2-cbc";
    static inline const std::string RC2_CFB = "rc2-cfb";
    static inline const std::string RC2_OFB = "rc2-ofb";

    static inline const std::string RC5_ECB = "rc5-ecb";
    static inline const std::string RC5_CBC = "rc5-cbc";
    static inline const std::string RC5_CFB = "rc5-cfb";
    static inline const std::string RC5_OFB = "rc5-ofb";

    /**
     * Create a new encrypter using the specified key.  The
     * key will be used to encrypt/decrypt values.
     */
    Encrypter( std::string_view scheme, std::string_view cryptKey );

    ~Encrypter();

    /**
     * Encrypt the specified string and return a base64 encoded
     * string representation of the encrypted characters using the
     * scheme used to create this instance.
     */
    std::string encrypt( std::string_view value );

    /**
     * Decrypt the specified base64 encoded encrypted characters
     * using the scheme used for this instance.
     */
    std::string decrypt( std::string_view value );

    // For pooling
    [[nodiscard]] bool valid() const { return true; }

  private:
    /** The scheme to use to initialise the encrypt/decrypt routines. */
    std::string scheme;

    /** The secret key to use to encrypt/decrypt data. */
    std::string key;

    /** A flag indicating that the encryption context needs refresh. */
    bool refreshEncryptContext = false;

    /** A flag indicating that the decryption context needs refresh. */
    bool refreshDecryptContext = false;

    EVP_CIPHER_CTX* encryptingContext = nullptr;
    EVP_CIPHER_CTX* decryptingContext = nullptr;

    static void loadOpenSSL();
    void initContext();
    void cleanOpenSSL();
    static void printError();
  };

}

#endif // ENCRYPTER_H
