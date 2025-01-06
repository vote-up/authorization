#pragma once

#include <string_view>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/server/http/http_request.hpp>
#include <userver/server/request/request_context.hpp>
#include "components/authentication/component.hpp"

namespace handlers::authorization_v1_signup {
class Handler final : public userver::server::handlers::HttpHandlerJsonBase {
 public:
  static constexpr std::string_view kName = "handler-authorization-v1-signup";

  Handler(const userver::components::ComponentConfig&,
          const userver::components::ComponentContext&);

  userver::formats::json::Value HandleRequestJsonThrow(
      const userver::server::http::HttpRequest&,
      const userver::formats::json::Value&,
      userver::server::request::RequestContext&) const override;

 private:
  const components::authentication::Component& _auth_manager;

  userver::formats::json::Value SignUp(
      const userver::server::http::HttpRequest&,
      const userver::formats::json::Value&) const;
};
}  // namespace handlers::authorization_v1_signup