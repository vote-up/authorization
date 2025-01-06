#include "jwt.hpp"
#include <fmt/format.h>
#include <string>
#include <string_view>
#include <userver/crypto/base64.hpp>
#include <userver/crypto/signers.hpp>
#include <userver/formats/json/serialize.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/formats/json/value_builder.hpp>

namespace utils::jwt {
namespace {
constexpr std::string_view kJwt = "JWT";

std::string CreateEncodedHeader(std::string_view algorithm) {
  userver::formats::json::ValueBuilder builder;
  builder["typ"] = kJwt;
  builder["alg"] = algorithm;
  return userver::crypto::base64::Base64Encode(
      userver::formats::json::ToString(builder.ExtractValue()),
      userver::crypto::base64::Pad::kWithout);
}

std::string CreateEncodedPayload(const userver::formats::json::Value& payload) {
  return userver::crypto::base64::Base64Encode(
      userver::formats::json::ToString(payload),
      userver::crypto::base64::Pad::kWithout);
}

std::string Encode(std::string_view encoded_header,
                   std::string_view encoded_payload,
                   const userver::crypto::Signer& signer) {
  const auto encoded_signature = userver::crypto::base64::Base64Encode(
      signer.Sign({encoded_header, ".", encoded_payload}),
      userver::crypto::base64::Pad::kWithout);
  return fmt::format("{}.{}.{}", encoded_header, encoded_payload,
                     encoded_signature);
}
}  // namespace

std::string Encode(const userver::formats::json::Value& payload,
                   const userver::crypto::Signer& signer) {
  const auto encoded_header = CreateEncodedHeader(signer.Name());
  const auto encoded_payload = CreateEncodedPayload(payload);
  return Encode(encoded_header, encoded_payload, signer);
}

}  // namespace utils::jwt