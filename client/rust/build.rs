fn main() {
  if cfg!(target_os = "macos")
  {
    cxx_build::bridge("src/lib.rs")
        .include("/usr/local/spt/include")
        .include("/usr/local/boost/include")
        .file("src/encrypter.cpp")
        .std("c++23")
        .compile("encrypter");

    println!("cargo::rustc-link-search=/usr/local/spt/lib");
  }
  else
  {
    cxx_build::bridge("src/lib.rs")
        .include("/opt/spt/include")
        .include("/opt/local/include")
        .file("src/encrypter.cpp")
        .std("c++23")
        .compile("encrypter");
    println!("cargo::rustc-link-search=/opt/spt/lib");
  }

  println!("cargo::rustc-link-lib=encrypter");
  println!("cargo::rustc-link-lib=encrypterapi");
  println!("cargo::rustc-link-lib=nanolog");
  println!("cargo:rerun-if-changed=src/encrypter.cpp");
  println!("cargo:rerun-if-changed=include/encrypter.hpp");
}