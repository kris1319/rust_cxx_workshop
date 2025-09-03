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

std::unique_ptr<Index> new_index(IndexType type);

////////////////////////////////////////////
///   Exercise 7: Exposing Index search
///
///   * Declare the shared type: struct IndexResult; Now you can use it!
///   * Define a function that searchs Index for a given word and returns a list of document names
///     in rust::Vec<rust::String>
///   * Define a function that searchs Index for a given word and returns a list of shared types:
///     the result structure defined in the bridge module
////////////////////////////////////////////

// C++ forward declaration
struct IndexResult;

// simpler version of search that uses vector bindings
rust::Vec<rust::String> search_index(const Index& index, rust::Str word);

// more complex version of search, uses shared `IndexResult` type that is defined in the bridge
rust::Vec<IndexResult> search_index_with_positions(const Index& index, rust::Str word);

////////////////////////////////////////////
///   Exercise 8: Exposing Storage async API as sync
///
///   * Below you can see already implemented shim functions
///   * .via() -- used to execute the future on a given executor
///   * inline executor is used in this example
////////////////////////////////////////////

void store_sync(
    std::shared_ptr<Storage> storage, const std::string& key, const std::string& value
) noexcept {
    auto executor = get_inline_executor();
    storage->store(key, value)
        .via(&executor)
        .wait();
}

rust::String fetch_sync(
    std::shared_ptr<Storage> storage, 
    const std::string& key
) noexcept {
    auto executor = get_inline_executor();

    std::string result;
    storage->fetch(key)
        .via(&executor)
        .thenValue([&result](auto&& res) mutable {
            if (res.hasValue()) {
                result = res.value();
            } else {
                // This is done for demonstration purposes only!
                // DO NOT handle not found cases with a placeholder value like "not found"
                // In production it should return something similar to an optional value:
                //   - string with contents associated with the given key
                //   - none if there are no value associated with the given key
                //   - error/exception if something bad happened
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
///             // `res` type is folly::Try which acts similar to the rust Result type
///             if (res.hasValue()) {
///                 // this branch handles case when Try is successful, ie no exceptions
///                 // thus we can return our "void" over the channel
///                 ok(std::move(tx));
///             } else {
///                 // this branch handles case when exception was raised
///                 // let's return the exception message over the channel
///                 fail(std::move(tx), rust::String(res.exception().what().c_str()));
///             }
///        });
///   * Note that the new store function you define should return `void`, ie nothing
///     That's because all the "return something" functionality is handled through 
///     rust channel. 
///   * Also note that the function is `noexcept`. That's because we explicitly capture
///     and handle exceptions within `thenTry` block
////////////////////////////////////////////

void store(
    std::shared_ptr<Storage> storage, 
    const std::string& key, 
    const std::string& value,
    rust::Fn<void(rust::Box<StoreTransmitter> tx)> ok,
    rust::Fn<void(rust::Box<StoreTransmitter> tx, rust::String str)> fail,
    rust::Box<StoreTransmitter> tx
) noexcept {
    auto executor = get_inline_executor();
    storage->store(key, value)
        .via(&executor)
        .thenTry([ok, fail, tx = std::move(tx)](auto&& res) mutable {
            if (res.hasValue()) {
                ok(std::move(tx));
            } else {
                fail(std::move(tx), rust::String(res.exception().what().c_str()));
            }
        });
}

////////////////////////////////////////////
///   Exercise 9.3: Exposing Storage async fetch
///
///   * You can start from copy-pasting the fetch sync function
///   * add callbacks as parameters:
///        - ok_found: rust::Fn<void(rust::Box<FetchTransmitter> tx, rust::String str)>
///        - ok_not_found: rust::Fn<void(rust::Box<FetchTransmitter> tx)>
///        - fail: rust::Fn<void(rust::Box<FetchTransmitter> tx, rust::String str)>
///   * add transmitter as a parameter:
///        - tx: rust::Box<FetchTransmitter>
///   * replace .wait() with .thenTry() like so
///        .thenTry([ok_found, ok_not_found, fail, tx = std::move(tx)](auto&& res) mutable {
///            if (res.hasValue()) {
///                // as mentioned earlier, res - is of a folly::Try type
///                auto maybe_data = res.value();
///                // ^ here we safely unwrap Try
///                // the successful value of the Try was in our case an Optional (same as in rust)
///
///                if (maybe_data.hasValue()) {
///                    // here we're checking that our Optional has value. and it does!
///                    // let's send it back to Rust over the channel using the callback
///                    ok_found(std::move(tx), rust::String(maybe_data.value()));
///                } else {
///                    // turns out the value was not found, we call out not_found callback
///                    ok_not_found(std::move(tx));
///                }
///            } else {
///                fail(std::move(tx), rust::String(res.exception().what().c_str()));
///            }
///        });
////////////////////////////////////////////

void fetch(
    std::shared_ptr<Storage> storage, 
    const std::string& key,
    rust::Fn<void(rust::Box<FetchTransmitter> tx, rust::String str)> ok_found,
    rust::Fn<void(rust::Box<FetchTransmitter> tx)> ok_not_found,
    rust::Fn<void(rust::Box<FetchTransmitter> tx, rust::String str)> fail,
    rust::Box<FetchTransmitter> tx
) noexcept {
    auto executor = get_inline_executor();
    storage->fetch(key)
        .via(&executor)
        .thenTry([ok_found, ok_not_found, fail, tx = std::move(tx)](auto&& res) mutable {
            if (res.hasValue()) {
                auto maybe_data = res.value();

                if (maybe_data.hasValue()) {
                    ok_found(std::move(tx), rust::String(maybe_data.value()));
                }else {
                    ok_not_found(std::move(tx));
                }
            } else {
                fail(std::move(tx), rust::String(res.exception().what().c_str()));
            }
        });
}
