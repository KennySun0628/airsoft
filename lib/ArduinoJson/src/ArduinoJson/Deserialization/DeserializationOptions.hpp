// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Deserialization/Filter.hpp>
#include <ArduinoJson/Deserialization/ingLimit.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TFilter>
struct DeserializationOptions {
  TFilter filter;
  DeserializationOption::ingLimit nestingLimit;
};

template <typename TFilter>
inline DeserializationOptions<TFilter> makeDeserializationOptions(
    TFilter filter, DeserializationOption::ingLimit nestingLimit = {}) {
  return {filter, nestingLimit};
}

template <typename TFilter>
inline DeserializationOptions<TFilter> makeDeserializationOptions(
    DeserializationOption::ingLimit nestingLimit, TFilter filter) {
  return {filter, nestingLimit};
}

inline DeserializationOptions<AllowAllFilter> makeDeserializationOptions(
    DeserializationOption::ingLimit nestingLimit = {}) {
  return {{}, nestingLimit};
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
