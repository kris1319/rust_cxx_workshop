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
