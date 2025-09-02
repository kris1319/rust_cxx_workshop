#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <folly/futures/Future.h>
#include <folly/executors/InlineExecutor.h>
#include <folly/Synchronized.h>

/// Simple key-value storage with asynchronous interface
/// Uses folly::Synchronized internally for thread safety
class Storage {
public:
    /// Create a new Storage instance
    static std::shared_ptr<Storage> init() {
        return std::make_shared<Storage>();
    }

    /// Store a key-value pair
    folly::SemiFuture<folly::Unit> store(std::string key, std::string value) {
        folly::Promise<folly::Unit> promise;
        try {
            _data.withWLock([&](auto& lockedMap) {
                lockedMap[key] = value;
            });
            promise.setValue();
        } catch (...) {
            promise.setException(folly::exception_wrapper(std::current_exception()));
        }

        return promise.getSemiFuture();
    }

    /// Fetch a value by key
    folly::SemiFuture<folly::Optional<std::string>> fetch(const std::string& key) {
        folly::Promise<folly::Optional<std::string>> promise;

        try {
            folly::Optional<std::string> result = _data.withRLock([&](const auto& lockedMap) {
                auto it = lockedMap.find(key);
                if (it != lockedMap.end()) {
                    return folly::make_optional(it->second);
                }
                return folly::Optional<std::string>();
            });

            promise.setValue(std::move(result));
        } catch (...) {
            promise.setException(folly::exception_wrapper(std::current_exception()));
        }

        return promise.getSemiFuture();
    }

private:
    folly::Synchronized<std::unordered_map<std::string, std::string>> _data;
};

/// Get a reference to the global inline executor
folly::InlineExecutor& get_inline_executor() {
    return folly::InlineExecutor::instance();
}