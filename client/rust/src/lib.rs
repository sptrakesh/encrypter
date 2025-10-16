#[cxx::bridge]
mod encrypter
{
  /// C++ API logging level options
  enum LogLevel
  {
    /// Log messages at the `critical` level.
    CRIT,
    /// Log messages at the `warning` level.
    WARN,
    /// Log messages at the `information` level.
    INFO,
    /// Log messages at the `debug` level.
    DEBUG
  }

  /// C++ API global logger configuration.
  struct Logger
  {
    /// The directory to which log files are written.  **Must** exist.  Trailing `/` is mandatory.
    path: String,
    /// The base/root name for the log files.  Logger will append day to the base name each day.
    name: String,
    /// The level to set for the logger.  Default is `LogLevel::INFO`.
    level: LogLevel,
    /// Set to `true` to also echo logs to `stdout`.  Default `false`.
    console: bool
  }

  struct Configuration
  {
    /// The host on which the service is running.
    host: String,
    /// The port on which the service is listening.
    port: u16
  }

  unsafe extern "C++"
  {
    include!("encrypter/include/encrypter.hpp");

    /// Initialise the C++ global logger.
    ///
    /// The logger is global for all SPT APIs, and should be initialised exactly once
    /// before initialising any clients APIs.  Ensure this if using the Rust wrapper alongside
    /// other wrappers (e.g. [encrypter](https://github.com/sptrakesh/config-db/tree/master/client/rust))
    ///
    /// # Arguments
    ///
    /// * `config` - The logging configuration structure
    pub fn init_logger(config: Logger);

    /// Initialise the C++ client API.  **Should** only be invoked **once**.
    ///
    /// # Arguments
    ///
    /// * `config` - The client configuration structure
    pub fn init(config: Configuration);

    /// Encrypt the specified data.
    ///
    /// # Arguments
    ///
    /// * `data` - The string data that is to be encrypted.
    ///
    /// # Returns
    /// - The encrypted value, or empty string if encryption failed.
    pub fn encrypt(data: &str) -> String;

    /// Decrypt the specified data.
    ///
    /// # Arguments
    ///
    /// * `data` - The string data that is to be decrypted.
    ///
    /// # Returns
    /// - The decrypted value, or empty string if decryption failed.
    pub fn decrypt(data: &str) -> String;
  }
}

impl encrypter::Logger
{
  /// Create a new logging configuration with default values for `level` and `console`.
  ///
  /// # Arguments
  ///
  /// * `path` - The path under which the log files are to be written.
  /// * `name` - The base name for the log files.
  pub fn new(dir: &str, name: &str) -> Self
  {
    encrypter::Logger{path: dir.to_string(), name: name.to_string(), level: encrypter::LogLevel::INFO, console: false}
  }
}

impl encrypter::Configuration
{
  /// Create a new instance of the API configuration using the specified values.
  ///
  /// # Arguments
  ///
  /// * `host` - The host on which the encrypter service is running.
  /// * `port` - The prot on which the encrypter service is listening.
  pub fn new(host: &str, port: u16) -> Self
  {
    encrypter::Configuration{host: host.to_string(), port}
  }
}

#[cfg(test)]
mod tests
{
  use crate::encrypter::{Configuration, Logger, LogLevel, decrypt, encrypt, init, init_logger};

  #[test]
  fn round_trip()
  {
    let mut conf = Logger::new("/tmp/", "encrypter-rust");
    conf.level = LogLevel::DEBUG;
    init_logger(conf);

    init(Configuration::new("localhost", 2030));

    let data =  "https://sptrakesh.github.io/encrypter.html";

    for i in 0..100
    {
      let data = format!("{}/{}", data, i);
      let res = encrypt(data.as_str());
      assert!(!res.is_empty());
      assert_ne!(res.as_str(), data);
      let res = decrypt(res.as_str());
      assert_eq!(res, data);
    }
  }
}