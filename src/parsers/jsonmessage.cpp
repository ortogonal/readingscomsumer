#include "jsonmessage.h"

namespace rc::parser {

std::optional<Json::Value> toJSONMessage(const std::string_view &str)
{
    const auto rawJsonLength = static_cast<int>(str.length());
    JSONCPP_STRING err;
    Json::Value root;

    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(str.data(), str.data() + rawJsonLength, &root, &err)) {
        return false;
    }
    return root;
}

} // namespace rc::parser
