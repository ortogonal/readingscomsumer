#include "accumulatedpower.h"

#include <algorithm>
#include <iostream>
#include <unordered_map>

namespace rc::consumer {

AccumulatedPower::AccumulatedPower(sw::redis::Redis &redis)
    : m_redis(redis)
{}

void AccumulatedPower::execute(const Json::Value &message)
{
    if (message["type"] == "han") {
        auto data = message["data"];

        auto id = data["id"].asString();
        std::transform(id.begin(), id.end(), id.begin(), [](char ch) {
            return ch == ' ' ? '_' : ch;
        });

        std::cout << "is obj " << data["input"].isObject() << std::endl;

        accumulatePower("acc:power:in:" + id, data["input"]);
        accumulatePower("acc:power:out:" + id, data["output"]);
    }
}

void AccumulatedPower::accumulatePower(const std::string &key, const Json::Value &powerMap)
{
    if (powerMap.isObject()) {
        if (powerMap["total"].isInt()) {
            auto wattsPerHour = (float)(powerMap["total"].asInt()) / 360.f;
            m_redis.incrbyfloat(key, wattsPerHour);
        }
    }
}

} // namespace rc::consumer
