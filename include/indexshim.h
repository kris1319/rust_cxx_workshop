#pragma once 

#include "rust_cxx/include/index.h"
#include "rust_cxx/include/storage.h"
#include "rust_cxx/src/main.rs.h"
#include "rust/cxx.h"

////////////////////////////////////////////////////////////
///          This is a shim layer for the C++ code
////////////////////////////////////////////////////////////


////////////////////////////////////////////
///   Exercise 3: Creating Index objects
///
///   * Implement a function that creates a new Index instance
///   * The function should have no arguments and return a smart pointer to the Index
///   * Use any IndexType you like as a default value for the Index constructor
////////////////////////////////////////////

////////////////////////////////////////////
///   Exercise 4: Paramentrised Index creation
///
///   * Make the Index creation function take IndexType as a parameter
////////////////////////////////////////////

////////////////////////////////////////////
///   Exercise 4: Paramentrised Index creation
///
///   * Make the Index creation function take IndexType as a parameter
////////////////////////////////////////////

////////////////////////////////////////////
///   Exercise 7: Exposing Index search
///
///   * Declare the shared type: struct IndexResult; Now you can use it!
///   * Define a function that searchs Index for a given word and returns a list of document names
///     in rust::Vec<rust::String>
///   * Define a function that searchs Index for a given word and returns a list of shared types:
///     the result structure defined in the bridge module
////////////////////////////////////////////

////////////////////////////////////////////
///   Exercise 8: Exposing Storage async API as sync
///
///   * Below you can see already implemented shim functions
///   * .via() -- used to execute the future on a given executor
///   * inline executor is used in this example
////////////////////////////////////////////

void store(
    std::shared_ptr<Storage> storage, const std::string& key, const std::string& value
) noexcept {
    auto executor = get_inline_executor();
    storage->store(key, value)
        .via(&executor)
        .wait();
}

rust::String fetch(
    std::shared_ptr<Storage> storage, 
    const std::string& key
) noexcept {
    auto executor = get_inline_executor();

    std::string result;
    storage->fetch(key)
        .via(&executor)
        .thenValue([&result](auto&& res) mutable {
            if (res.hasValue()) {
                result = res.value()
            } else {
                result = "not found";
            }
        });

    return rust::String(result);
}

////////////////////////////////////////////
///   Exercise 9.2: Exposing Storage async store
///
///   * You can start from copy-pasting the store sync function
///   * add callbacks as parameters:
///        - ok: rust::Fn<void(rust::Box<StoreTransmitter> tx)>
///        - fail: rust::Fn<void(rust::Box<StoreTransmitter> tx, rust::String str)>
///   * add transmitter as a parameter:
///        - tx: rust::Box<StoreTransmitter>
///   * replace .wait() with .thenTry() like so
///        .thenTry([ok, fail, tx = std::move(tx)](auto&& res) mutable {
///             if (res.hasValue()) {
///                 // the value is void
///                 ok(std::move(tx));
///             } else {
///                 fail(std::move(tx), rust::String(res.exception().what().c_str()));
///             }
///        });
////////////////////////////////////////////

////////////////////////////////////////////
///   Exercise 9.3: Exposing Storage async fetch
///
///   * You can start from copy-pasting the fetch sync function
///   * add callbacks as parameters:
///        - ok_found: rust::Fn<void(rust::Box<FetchTransmitter> tx)>
///        - ok_not_found: rust::Fn<void(rust::Box<FetchTransmitter> tx)>
///        - fail: rust::Fn<void(rust::Box<FetchTransmitter> tx, rust::String str)>
///   * add transmitter as a parameter:
///        - tx: rust::Box<FetchTransmitter>
///   * replace .wait() with .thenTry() like so
///        .thenTry([ok_found, ok_not_found, fail, tx = std::move(tx)](auto&& res) mutable {
///            if (res.hasValue()) {
///                auto maybe_data = res.value();
///
///                if (maybe_data.hasValue()) {
///                    ok_found(std::move(tx), rust::String(maybe_data.value()));
///                }else {
///                    ok_not_found(std::move(tx));
///                }
///            } else {
///                fail(std::move(tx), rust::String(res.exception().what().c_str()));
///            }
///        });
////////////////////////////////////////////
