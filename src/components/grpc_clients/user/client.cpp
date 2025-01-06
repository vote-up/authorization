#include "client.hpp"
#include <grpcpp/client_context.h>
#include <handlers/user.pb.h>
#include <handlers/user_client.usrv.pb.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <userver/components/component_base.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/engine/deadline.hpp>
#include <userver/ugrpc/client/client_factory_component.hpp>
#include <userver/utils/datetime/date.hpp>
#include <userver/utils/string_to_duration.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/yaml_config/schema.hpp>

namespace components::user_service {

namespace {
handlers::api::users::v1::Gender StringToGender(std::string_view gender_str) {
  if (gender_str == "male") {
    return handlers::api::users::v1::MALE;
  }
  if (gender_str == "female") {
    return handlers::api::users::v1::FEMALE;
  }
  throw std::runtime_error("Wrong gender value");
}
}  // namespace

Client::Client(const userver::components::ComponentConfig& config,
               const userver::components::ComponentContext& component_context)
    : userver::components::LoggableComponentBase(config, component_context),
      _client_factory(
          component_context
              .FindComponent<userver::ugrpc::client::ClientFactoryComponent>()
              .GetFactory()),
      _client(_client_factory
                  .MakeClient<handlers::api::users::v1::UserServiceClient>(
                      std::string{kName.data(), kName.size()},
                      config["endpoint"].As<std::string>())),
      _deadline(userver::utils::StringToDuration(
          config["deadline"].As<std::string>())) {}

handlers::api::users::v1::CreateUserResponse Client::CreateUser(
    const dto::authorization::CreateUser& user) const {
  handlers::api::users::v1::CreateUserRequest request{};

  request.set_phone_number(user.phoneNumber);
  request.set_full_name(user.fullName);
  request.set_birth_date(userver::utils::datetime::ToString(user.birthDate));
  request.set_gender(StringToGender(user.gender));

  auto context = std::make_unique<grpc::ClientContext>();
  context->set_deadline(userver::engine::Deadline::FromDuration(_deadline));

  return _client.CreateUser(request).Finish();
}

handlers::api::users::v1::User Client::TakeUserByPhoneNumber(
    const std::string& phone_number) const {
  handlers::api::users::v1::TakeUserByPhoneNumberRequest request{};

  request.set_phone_number(phone_number);

  auto context = std::make_unique<grpc::ClientContext>();
  context->set_deadline(userver::engine::Deadline::FromDuration(_deadline));

  return _client.TakeUserByPhoneNumber(request).Finish();
}

userver::yaml_config::Schema Client::GetStaticConfigSchema() {
  return userver::yaml_config::MergeSchemas<
      userver::components::LoggableComponentBase>(R"(
type: object
description: >
    a user-defined grpc client
additionalProperties: false
properties:
    endpoint:
        type: string
        description: >
            Endpoint of user_service server
    deadline:
        type: string
        description: >
            GRPc request deadline
)");
}

}  // namespace components::user_service