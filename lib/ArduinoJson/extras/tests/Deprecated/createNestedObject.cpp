// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include <string>

#include "Literals.hpp"

TEST_CASE("JsonDocument::createedObject()") {
  JsonDocument doc;

  SECTION("createedObject()") {
    JsonObject object = doc.createedObject();
    object["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "[{\"hello\":\"world\"}]");
  }

  SECTION("createedObject(const char*)") {
    JsonObject object = doc.createedObject("key");
    object["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "{\"key\":{\"hello\":\"world\"}}");
  }

  SECTION("createedObject(std::string)") {
    JsonObject object = doc.createedObject("key"_s);
    object["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "{\"key\":{\"hello\":\"world\"}}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("createedObject(VLA)") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "key");
    JsonObject object = doc.createedObject(vla);
    object["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "{\"key\":{\"hello\":\"world\"}}");
  }
#endif
}

TEST_CASE("JsonArray::createedObject()") {
  JsonDocument doc;
  JsonArray array = doc.to<JsonArray>();
  JsonObject object = array.createedObject();
  object["hello"] = "world";
  REQUIRE(doc.as<std::string>() == "[{\"hello\":\"world\"}]");
}

TEST_CASE("JsonObject::createedObject()") {
  JsonDocument doc;
  JsonObject object = doc.to<JsonObject>();

  SECTION("createedObject(const char*)") {
    JsonObject nestedObject = object.createedObject("key");
    nestedObject["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "{\"key\":{\"hello\":\"world\"}}");
  }

  SECTION("createedObject(std::string)") {
    JsonObject nestedObject = object.createedObject("key"_s);
    nestedObject["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "{\"key\":{\"hello\":\"world\"}}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("createedObject(VLA)") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "key");
    JsonObject nestedObject = object.createedObject(vla);
    nestedObject["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "{\"key\":{\"hello\":\"world\"}}");
  }
#endif
}

TEST_CASE("JsonVariant::createedObject()") {
  JsonDocument doc;
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("createedObject()") {
    JsonObject object = variant.createedObject();
    object["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "[{\"hello\":\"world\"}]");
  }

  SECTION("createedObject(const char*)") {
    JsonObject object = variant.createedObject("key");
    object["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "{\"key\":{\"hello\":\"world\"}}");
  }

  SECTION("createedObject(std::string)") {
    JsonObject object = variant.createedObject("key"_s);
    object["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "{\"key\":{\"hello\":\"world\"}}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("createedObject(VLA)") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "key");
    JsonObject object = variant.createedObject(vla);
    object["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "{\"key\":{\"hello\":\"world\"}}");
  }
#endif
}
