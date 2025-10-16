# Rust Client
A simple Rust wrapper around the C++ API using [cxx.rs](https://cxx.rs/).

See [build.rs](build.rs) for setting include search paths and library search paths and libraries.

## Use
Include the checked out directory into your project workspace, or copy the sources into your
project and use as appropriate.

```rust
use encrypter::{decrypt, encrypt, init, Configuration, LogLevel};

#[test]
fn round_trip()
{
  let mut conf = Logger::new("/tmp/", "encrypter-rust");
  conf.level = LogLevel::DEBUG;
  init_logger(conf);

  init(Configuration::new("localhost", 2030));
  
  let data =  "https://sptrakesh.github.io/encrypter.html";
  let res = encrypt(data);
  assert!(!res.is_empty());
  assert_ne!(res.as_str(), data);
  let res = decrypt(res.as_str());
  assert_eq!(res, data);
}
```