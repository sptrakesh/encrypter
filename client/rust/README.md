# Rust Client
A simple Rust wrapper around the C++ API using [cxx.rs](https://cxx.rs/).

## Use
Include the checked out directory into your project workspace, or copy the sources into your
project and use as appropriate.

```rust
use crate::encrypter::{decrypt, encrypt, init, Configuration, LogLevel};

#[test]
fn round_trip()
{
  let mut conf = Configuration::new("localhost", 2030);
  conf.log_level = LogLevel::DEBUG;
  init(&conf);
  let data =  "https://sptrakesh.github.io/encrypter.html";
  let res = encrypt(data);
  assert!(!res.is_empty());
  assert_ne!(res.as_str(), data);
  let res = decrypt(res.as_str());
  assert_eq!(res, data);
}
```