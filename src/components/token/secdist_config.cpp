#include "secdist_config.hpp"
#include <string>
#include <userver/formats/json/value.hpp>

namespace components::token::secdist {
Secrets::Secrets(const userver::formats::json::Value& doc)
    : access_token_public_key(
          doc["service"]["access_token"]["public_key"].As<std::string>()),
      access_token_private_key(
          doc["service"]["access_token"]["private_key"].As<std::string>()) {}
}  // namespace components::token::secdist