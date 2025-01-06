#include "handler.hpp"
#include <userver/chaotic/exception.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/http/status_code.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/server/http/http_method.hpp>
#include <userver/ugrpc/client/exceptions.hpp>
#include "components/authentication/component.hpp"
#include "docs/yaml/openapi.hpp"

namespace handlers::authorization_v1_signup {
Handler::Handler(const userver::components::ComponentConfig& config,
                 const userver::components::ComponentContext& component_context)
    : userver::server::handlers::HttpHandlerJsonBase(config, component_context),
      _auth_manager(
          component_context
              .FindComponent<components::authentication::Component>()) {}

userver::formats::json::Value Handler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    userver::server::request::RequestContext&) const {
  switch (request.GetMethod()) {
    case userver::server::http::HttpMethod::kPost:
      return SignUp(request, request_json);
    default:
      throw ClientError(ExternalBody{std::format("method-not-allowed")});
  }
}

userver::formats::json::Value Handler::SignUp(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json) const {
  try {
    const auto& user = request_json.As<dto::authorization::CreateUser>();
    const auto& access_token = _auth_manager.SignUp(user);
    return userver::formats::json::ValueBuilder{access_token}.ExtractValue();
  } catch (const userver::chaotic::Error& ex) {
    LOG_WARNING() << ex.what();
    request.SetResponseStatus(userver::http::kBadRequest);
    return {};
  } catch (const userver::ugrpc::client::AlreadyExistsError& ex) {
    LOG_WARNING() << ex.what();
    request.SetResponseStatus(userver::http::kConflict);
    return {};
  } catch (const userver::ugrpc::client::NotFoundError& ex) {
    LOG_WARNING() << ex.what();
    request.SetResponseStatus(userver::http::kNotFound);
    return {};
  }
}
}  // namespace handlers::authorization_v1_signup