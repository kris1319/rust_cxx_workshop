#include "rust_cxx/include/indexshim.h"

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

std::unique_ptr<Index> new_index(IndexType type) {
  return std::make_unique<Index>(type);
}

////////////////////////////////////////////
///   Exercise 7: Exposing Index search
///
///   * Define a function that searchs Index for a given word and returns a list of document names
///     in rust::Vec<rust::String>
///   * Define a function that searchs Index for a given word and returns a list of shared types:
///     the result structure defined in the bridge module
///
///   * rust::Vec has push_back method to add elements
///   * rust::String can be constructed from a const char* or std::string
///   * (std::string)rust::Str can be used to convert rust::Str to std::string
////////////////////////////////////////////

rust::Vec<rust::String> search_index(const Index& index, rust::Str word) {
    auto results = index.search((std::string)word);
    rust::Vec<rust::String> rust_results;
    for (const auto& [doc_name, positions] : results) {
        rust_results.push_back(doc_name);
    }
    return rust_results;
}

rust::Vec<IndexResult> search_index_with_positions(const Index& index, rust::Str word) {
    auto results = index.search((std::string)word);
    rust::Vec<IndexResult> rust_results;
    for (const auto& [doc_name, positions] : results) {
        rust::Vec<unsigned> rust_positions;
        for (size_t pos : positions) {
            rust_positions.push_back(static_cast<unsigned>(pos));
        }
        rust_results.push_back(IndexResult{doc_name, std::move(rust_positions)});
    }
    
    return rust_results;
}
