use anyhow::Error;
use anyhow::Result;

use cxx::let_cxx_string;

use futures::channel::oneshot;

////////////////////////////////////////////////////////////////
// This is a bridge module for interfacing with C++ code using the cxx crate.
////////////////////////////////////////////////////////////////

#[cxx::bridge]
mod ffi {
    //////////////////////////////////////////////////////////////
    // Here go your shared structs and enums
    //////////////////////////////////////////////////////////////

    extern "Rust" {
        // your code goes here
    }

    unsafe extern "C++" {
        include!("rust_cxx/include/indexshim.h");

        // your code goes here
    }

    //////////////////////////////////////////////////////////////////
    //    Exercise 1: Exposing Index class to Rust
    //
    //    * Declare an opaque type for the C++ class `Index` in the corresponding extern "C++" block
    //    * Use `type` key word to declare the type
    //////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////
    //    Exercise 2: Creating Index objects
    //
    //    * Add signature for the C++ Index constructor
    //    * You can use #[Self = "Index"] attribute to indicate a static member function
    //    * Function signarute in the bridge is a normal Rust function and matches 
    //      the C++ function signature
    //
    //    p.s. This should fail :)
    //////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////
    //    Exercise 3: Creating Index objects
    //
    //    * Add signature for the C++ Index creation function that returns a smart pointer
    //    * Use cxx::UniquePtr or cxx::SharedPtr correspondingly
    //    * Now create the Index object in the main function
    //////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////
    //    Exercise 4: Paramentrised Index creation
    //
    //    * Define an enum IndexType in the bridge module that matches the C++ enum 
    //    * Add #[repr(i32)] attribute to ensure the enum is represented as a 32-bit integer
    //    * You can also derive Clone, Copy and Debug traits for convenience, cxx will handle them for you
    //    * Add IndexType declaration to the corresponding extern "C++" block to map it with the C++ enum
    //    * Modify the Index creation function to take IndexType as a parameter
    //    * Create Index object with different IndexType values in the main function
    //////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////
    //    Exercise 5: Exposing const Index methods
    //
    //    * Add signatures for index_type() and dictionary_size() methods
    //    * Call them in the main function and print the results
    //////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////
    //    Exercise 6: Exposing non-const Index methods: indexing
    //
    //    * Add signatures for the index() method
    //    * Use Pin<&mut Index> for the self parameter to indicate a mutable method
    //    * Call it in the main function and print the results
    //////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////
    //    Exercise 7: Exposing Index search
    //
    //    * Define a shared struct to hold the results of the search
    //    * Declare function signatures for the search shim functions
    //////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////
    //    Example async signatures
    //////////////////////////////////////////////////////////////////
    // unsafe extern "C++" {
    //     include!("rust_cxx/include/storage.h");
    //     type Storage;

    //     #[Self = "Storage"]
    //     fn init() -> SharedPtr<Storage>;

    //     fn store(
    //         storage: SharedPtr<Storage>, 
    //         key: &CxxString, 
    //         value: &CxxString,
    //         ok: fn(ctx: Box<StoreTransmitter>),
    //         fail: fn(ctx: Box<StoreTransmitter>, err: String),
    //         ctx: Box<StoreTransmitter>,
    //     );

    //     fn fetch(
    //         storage: SharedPtr<Storage>, 
    //         key: &CxxString, 
    //         ok_found: fn(ctx: Box<FetchTransmitter>, result: String),
    //         ok_not_found: fn(ctx: Box<FetchTransmitter>),
    //         fail: fn(ctx: Box<FetchTransmitter>, err: String),
    //         ctx: Box<FetchTransmitter>,
    //     );
    // }
}

//////////////////////////////////////////////////////////////////
//    Exercise 9.1: Define a oneshot channel and the ok/fail callbacks
//
//    * Look at the example on the slides
//    * Expose the transmitter struct to C++ via extern "Rust" block
//    * Define the ok and fail callbacks that send the results back via the channel
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//    Exercise 9.2: Define an async function store_thing that uses the channel
//
//    * Expose defined shim async store function to Rust via the bridge module
//    * Implement the async function that uses the channel to wait for the result
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//    Example
//////////////////////////////////////////////////////////////////
// struct StoreTransmitter(oneshot::Sender<Result<()>>);
// fn store_ok(tx: Box<StoreTransmitter>) {
//     let _ = tx.0.send(Ok(()));
// }
// fn store_fail(tx: Box<StoreTransmitter>, err: String) {
//     let err = Error::msg(err);
//     let _ = tx.0.send(Err(err));
// }
// async fn store_thing(
//     storage: cxx::SharedPtr<ffi::Storage>, 
//     key: &cxx::CxxString, 
//     value: &cxx::CxxString,
// ) -> Result<()> {
//     let (tx, rx) = oneshot::channel();
//     let tx = Box::new(StoreTransmitter(tx));
//     ffi::store(storage, key, value, store_ok, store_fail, tx);
//     rx.await?   
// }

#[tokio::main]
async fn main() -> Result<()> {
    println!("Hello, Rustaceans!");

    // Your code goes here
    //
    // let mut index = ffi::new_index(ffi::IndexType::HashMap);
    // println!("Index size: {:?}", index.dictionary_size());
    //
    // let_cxx_string!(name = "doc1");
    // let_cxx_string!(text = "This is a test document with 10 words to index.");
    //
    // index.pin_mut().index(&name, &text);
    //
    // println!("Index size after indexing: {:?}", index.dictionary_size());

    // let_cxx_string!(name = "doc2");
    // let_cxx_string!(text = "This is a second test document with some random ending.");
    // index.pin_mut().index(&name, &text);
    // println!("Index size after 2 indexing: {:?}", index.dictionary_size());

    // let docs = ffi::search_index(&index, "test");
    // println!("Search results for 'test': {:?}", docs);

    // let results = ffi::search_index_with_positions(&index, "test");
    // for result in results {
    //     println!("Document: {}, Positions: {:?}", result.name, result.positions);
    // }

    // More examples:
    
    // println!("Initialising storage");
    // let storage = ffi::Storage::init();

    // store_thing(storage.clone(), &name, &text).await?;
    // println!("Stored into storage");

    // let fetched = fetch_thing(storage, &name).await?;
    // println!("Fetched from storage: {:?}", fetched);

    Ok(())
}
