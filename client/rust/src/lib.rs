#[cxx::bridge]
mod encrypter
{
  enum LogLevel
  {
    CRIT,
    WARN,
    INFO,
    DEBUG
  }

  struct Configuration
  {
    host: String,
    port: u16,
    log_level: LogLevel,
    log_to_console: bool
  }

  unsafe extern "C++"
  {
    include!("encrypter/include/encrypter.hpp");

    pub fn init(config: &Configuration);
    pub fn encrypt(data: &str) -> String;
    pub fn decrypt(data: &str) -> String;
  }
}

impl encrypter::Configuration
{
  pub fn new(host: &str, port: u16) -> Self
  {
    encrypter::Configuration{host: host.to_string(), port, log_level: encrypter::LogLevel::INFO, log_to_console: false}
  }
}

#[cfg(test)]
mod tests
{
  use crate::encrypter::{decrypt, encrypt, init, Configuration, LogLevel};

  #[test]
  fn round_trip()
  {
    let mut conf = Configuration::new("localhost", 2030);
    conf.log_level = LogLevel::DEBUG;
    init(&conf);
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