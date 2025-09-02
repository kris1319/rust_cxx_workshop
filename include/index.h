#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

/// Type of index to use, there is only one implementation however.
/// This serves as a demonstration for enum interop.
enum class IndexType {
    Tree,
    HashMap,
};

class Index {
public:
    using WordIndex = std::unordered_map<std::string, std::vector<size_t>>;

    Index(IndexType type): _type(type) {}

    /// Number of unique words in the index
    int dictionary_size() const {
        return _index.size();
    }

    /// Return the type of the index
    IndexType index_type() const {
        return _type;
    }

    /// Given a document name and its text, index all words in the text
    void index(const std::string& name, const std::string& text) {
        size_t i = 0;
        while (i < text.size()) {
            // Skip non-alpha characters
            while (i < text.size() && !(std::isalpha(text[i]) || std::isdigit(text[i]))) ++i;
            size_t start = i;
            // Find end of word
            while (i < text.size() && (std::isalpha(text[i]) || std::isdigit(text[i]))) ++i;
            if (start < i) {
                std::string word = text.substr(start, i - start);
                _index[word][name].push_back(start);
            }
        }
    }

    /// For the given word return all documents and positions it occurs at
    WordIndex search(const std::string& word) const {
        auto it = _index.find(word);
        if (it != _index.end()) {
            return it->second;
        }
        return {};
    }

private:
    IndexType _type;
    std::unordered_map<std::string, WordIndex> _index;
};
