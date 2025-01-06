#pragma once

#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <userver/components/component_base.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/schema.hpp>
#include "secdist_config.hpp"

namespace components::token {
class Component final : public userver::components::LoggableComponentBase {
 public:
  static constexpr std::string_view kName = "component-token";
  Component(const userver::components::ComponentConfig& config,
            const userver::components::ComponentContext& component_context);

  std::string Create(const boost::uuids::uuid&) const;

  static userver::yaml_config::Schema GetStaticConfigSchema();

 private:
  const secdist::Secrets _secrets;
  std::chrono::milliseconds _access_token_ttl;
};

}  // namespace components::token