#pragma once

#include <boost/uuid/uuid.hpp>
#include <string>
#include <userver/components/component_base.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include "components/grpc_clients/user/client.hpp"
#include "components/token/component.hpp"
#include "docs/yaml/openapi.hpp"

namespace components::authentication {
class Component final : public userver::components::LoggableComponentBase {
 public:
  static constexpr std::string_view kName = "component-authentication";
  Component(const userver::components::ComponentConfig&,
            const userver::components::ComponentContext&);

  std::string SignIn(const std::string&) const;

  std::string SignUp(const dto::authorization::CreateUser&) const;

 private:
  const components::token::Component& _token_manager;
  const components::user_service::Client& _user_service_client;

  std::string Authenticate(const boost::uuids::uuid& user_id) const;
};
}  // namespace components::authentication