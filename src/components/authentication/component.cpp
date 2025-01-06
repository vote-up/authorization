#include "component.hpp"
#include <boost/uuid/uuid.hpp>
#include <string>
#include <string_view>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include "components/grpc_clients/user/client.hpp"
#include "components/token/component.hpp"
#include "userver/storages/postgres/component.hpp"

namespace components::authentication {
Component::Component(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context)
    : userver::components::LoggableComponentBase(config, component_context),
      _token_manager(
          component_context.FindComponent<components::token::Component>()),
      _user_service_client(
          component_context.FindComponent<components::user_service::Client>()) {
}

std::string Component::Authenticate(const boost::uuids::uuid& user_id) const {
  return _token_manager.Create(user_id);
}

std::string Component::SignIn(const std::string& phone_number) const {
  auto response = _user_service_client.TakeUserByPhoneNumber(phone_number);

  return Authenticate(userver::utils::BoostUuidFromString(response.id()));
}

std::string Component::SignUp(
    const dto::authorization::CreateUser& user_info) const {
  auto response = _user_service_client.CreateUser(user_info);

  return Authenticate(userver::utils::BoostUuidFromString(response.user_id()));
}
}  // namespace components::authentication