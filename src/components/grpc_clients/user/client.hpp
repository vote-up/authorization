#pragma once

#include <handlers/user.pb.h>
#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <handlers/user_client.usrv.pb.hpp>
#include <string>
#include <string_view>
#include <userver/components/component_base.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/ugrpc/client/client_factory.hpp>
#include "docs/yaml/openapi.hpp"

namespace components::user_service {
class Client final : public userver::components::LoggableComponentBase {
 public:
  static constexpr std::string_view kName = "client-user-service";

  Client(const userver::components::ComponentConfig&,
         const userver::components::ComponentContext&);

  handlers::api::users::v1::CreateUserResponse CreateUser(
      const dto::authorization::CreateUser& user) const;

  handlers::api::users::v1::User TakeUserByPhoneNumber(
      const std::string& phone_number) const;

  static userver::yaml_config::Schema GetStaticConfigSchema();

 private:
  userver::ugrpc::client::ClientFactory& _client_factory;
  handlers::api::users::v1::UserServiceClient _client;

  std::chrono::milliseconds _deadline;
};
}  // namespace components::user_service