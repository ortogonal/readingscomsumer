#include "accumulatedprice.h"

#include <iostream>

namespace rc::consumer {

AccumulatedPrice::AccumulatedPrice(sw::redis::Redis &redis,
                                   provider::IDayAheadPrice &dayAheadProvider)
    : m_redis(redis)
    , m_dayAheadProvider(dayAheadProvider)
{}

void AccumulatedPrice::execute(const Json::Value &message)
{
    if (message["type"] == "han") {
        auto data = message["data"];

        auto id = data["id"].asString();
        std::transform(id.begin(), id.end(), id.begin(), [](char ch) {
            return ch == ' ' ? '_' : ch;
        });
        auto hour = toHour(data["timestamp"].asString());
        auto price = m_dayAheadProvider.priceAt(hour);

        accumulatePrice("acc:price:in:" + id, price, data["input"]);
        accumulatePrice("acc:price:out:" + id, price, data["output"]);
    }
}

provider::Hour AccumulatedPrice::toHour(const std::string ts) const
{
    auto year = 2000 + std::stoi(ts.substr(0, 2));
    auto month = std::stoi(ts.substr(2, 2));
    auto day = std::stoi(ts.substr(4, 2));
    auto hour = std::stoi(ts.substr(6, 2));

    return {year, month, day, hour};
}

void AccumulatedPrice::accumulatePrice(const std::string &key,
                                       const float &price,
                                       const Json::Value &powerMap)
{
    if (powerMap.isObject()) {
        if (powerMap["total"].isInt()) {
            auto wattsPerHour = (float) (powerMap["total"].asInt()) / 360.f;
            auto cost = wattsPerHour * (price / 1000);
            m_redis.incrbyfloat(key, cost);
        }
    }
}

} // namespace rc::consumer
