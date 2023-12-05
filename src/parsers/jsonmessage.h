#pragma once

#include <optional>
#include <string_view>

#include <json/json.h>

namespace rc::parser {

std::optional<Json::Value> toJSONMessage(const std::string_view &str);

}
