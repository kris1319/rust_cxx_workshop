fn main() {
    cxx_build::bridge("src/main.rs")
        .file("src/indexshim.cpp")
        .include("/opt/homebrew/include")
        .std("c++17")
        .compile("rust_cxx");

    println!("cargo:rustc-link-search=native=/opt/homebrew/lib");

    println!("cargo:rustc-link-lib=static=folly");

    println!("cargo:rustc-link-lib=static=fmt");
    println!("cargo:rustc-link-lib=static=glog");
    println!("cargo:rustc-link-lib=static=gflags");
    println!("cargo:rustc-link-lib=static=lz4");
    println!("cargo:rustc-link-lib=static=z");
    println!("cargo:rustc-link-lib=static=double-conversion");
    println!("cargo:rustc-link-lib=static=event");
    println!("cargo:rustc-link-lib=static=snappy");
    println!("cargo:rustc-link-lib=static=event_pthreads");
    println!("cargo:rustc-link-lib=static=boost_context");

    println!("cargo:rerun-if-changed=include/index.h");
    println!("cargo:rerun-if-changed=include/indexshim.h");
    println!("cargo:rerun-if-changed=include/storage.h");
}