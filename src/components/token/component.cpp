#include "component.hpp"
#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <userver/components/component_base.hpp>
#include <userver/crypto/signers.hpp>
#include <userver/formats/json/serialize.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/storages/secdist/component.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/utils/datetime.hpp>
#include <userver/utils/string_to_duration.hpp>
#include <userver/utils/strong_typedef.hpp>
#include <userver/yaml_config/schema.hpp>
#include "secdist_config.hpp"
#include "userver/formats/parse/boost_uuid.hpp"
#include "userver/yaml_config/merge_schemas.hpp"
#include "utils/jwt.hpp"

namespace components::token {
Component::Component(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context)
    : userver::components::LoggableComponentBase(config, component_context),
      _secrets(component_context.FindComponent<userver::components::Secdist>()
                   .Get()
                   .Get<secdist::Secrets>()),
      _access_token_ttl(userver::utils::StringToDuration(
          config["access_token_ttl"].As<std::string>())) {}

std::string Component::Create(const boost::uuids::uuid& user_id) const {
  userver::formats::json::ValueBuilder payload;
  payload["sub"] = userver::utils::ToString(user_id);
  payload["exp"] = userver::utils::datetime::Now() + _access_token_ttl;
  return utils::jwt::Encode(
      payload.ExtractValue(),
      userver::crypto::SignerPs512(
          _secrets.access_token_private_key.GetUnderlying()));
}

userver::yaml_config::Schema Component::GetStaticConfigSchema() {
  return userver::yaml_config::MergeSchemas<
      userver::components::LoggableComponentBase>(R"~(
type: object
description: token time to expire
additionalProperties: false
properties:
    access_token_ttl:
        type: string
        description: access token time to live
)~");
}
}  // namespace components::token