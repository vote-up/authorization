#pragma once

#include <userver/crypto/base64.hpp>
#include <userver/crypto/signers.hpp>
#include <userver/formats/json/value.hpp>

namespace utils::jwt {
std::string Encode(const userver::formats::json::Value& payload,
                   const userver::crypto::Signer& signer);
}