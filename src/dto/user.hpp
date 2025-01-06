#pragma once

#include <string>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/parse/to.hpp>
#include <userver/formats/serialize/to.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/utils/datetime/date.hpp>

namespace dto {
struct User {
  std::string phone_number;
  std::string full_name;
  userver::utils::datetime::Date birth_date;
};

template <class Value>
User Parse(const Value& data, userver::formats::parse::To<User>) {
  return {.phone_number = data["phoneNumber"].template As<std::string>(),
          .full_name = data["fullName"].template As<std::string>(),
          .birth_date =
              data["birthDate"].template As<userver::utils::datetime::Date>()};
}
}  // namespace dto