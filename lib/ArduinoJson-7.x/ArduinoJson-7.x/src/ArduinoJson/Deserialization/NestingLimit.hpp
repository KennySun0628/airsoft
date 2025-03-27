// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

namespace DeserializationOption {
class ingLimit {
 public:
  ingLimit() : value_(ARDUINOJSON_DEFAULT_NESTING_LIMIT) {}
  explicit ingLimit(uint8_t n) : value_(n) {}

  ingLimit decrement() const {
    ARDUINOJSON_ASSERT(value_ > 0);
    return ingLimit(static_cast<uint8_t>(value_ - 1));
  }

  bool reached() const {
    return value_ == 0;
  }

 private:
  uint8_t value_;
};
}  // namespace DeserializationOption

ARDUINOJSON_END_PUBLIC_NAMESPACE
