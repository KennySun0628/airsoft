// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include <string>

#include "Literals.hpp"

TEST_CASE("JsonDocument::createedArray()") {
  JsonDocument doc;

  SECTION("createedArray()") {
    JsonArray array = doc.createedArray();
    array.add(42);
    REQUIRE(doc.as<std::string>() == "[[42]]");
  }

  SECTION("createedArray(const char*)") {
    JsonArray array = doc.createedArray("key");
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }

  SECTION("createedArray(std::string)") {
    JsonArray array = doc.createedArray("key"_s);
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("createedArray(VLA)") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "key");
    JsonArray array = doc.createedArray(vla);
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }
#endif
}

TEST_CASE("JsonArray::createedArray()") {
  JsonDocument doc;
  JsonArray array = doc.to<JsonArray>();
  JsonArray nestedArray = array.createedArray();
  nestedArray.add(42);
  REQUIRE(doc.as<std::string>() == "[[42]]");
}

TEST_CASE("JsonObject::createedArray()") {
  JsonDocument doc;
  JsonObject object = doc.to<JsonObject>();

  SECTION("createedArray(const char*)") {
    JsonArray array = object.createedArray("key");
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }

  SECTION("createedArray(std::string)") {
    JsonArray array = object.createedArray("key"_s);
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("createedArray(VLA)") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "key");
    JsonArray array = object.createedArray(vla);
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }
#endif
}

TEST_CASE("JsonVariant::createedArray()") {
  JsonDocument doc;
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("createedArray()") {
    JsonArray array = variant.createedArray();
    array.add(42);
    REQUIRE(doc.as<std::string>() == "[[42]]");
  }

  SECTION("createedArray(const char*)") {
    JsonArray array = variant.createedArray("key");
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }

  SECTION("createedArray(std::string)") {
    JsonArray array = variant.createedArray("key"_s);
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("createedArray(VLA)") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "key");
    JsonArray array = variant.createedArray(vla);
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }
#endif
}
