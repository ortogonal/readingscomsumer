#include "livepower.h"

#include <algorithm>
#include <iostream>
#include <unordered_map>

namespace rc::consumer {

LivePower::LivePower(sw::redis::Redis &redis)
    : m_redis(redis)
{}

void LivePower::execute(const Json::Value &message)
{
    if (message["type"] == "han") {
        auto data = message["data"];

        auto id = data["id"].asString();
        std::transform(id.begin(), id.end(), id.begin(), [](char ch) {
            return ch == ' ' ? '_' : ch;
        });

        setLivePower("live:power:in:" + id, data["input"]);
        setLivePower("live:power:out:" + id, data["output"]);

        // TODO: Publish on livepower-channel
    }
}

void LivePower::setLivePower(const std::string &key, const Json::Value &powerJsonMap)
{
    auto total = powerJsonMap["total"];
    auto l1 = powerJsonMap["l1"];
    auto l2 = powerJsonMap["l2"];
    auto l3 = powerJsonMap["l3"];

    std::unordered_map<std::string, std::string> power = {
        {"total", total.asString()},
        {"l1", l1.asString()},
        {"l2", l2.asString()},
        {"l3", l3.asString()},
    };

    m_redis.hmset(key, power.begin(), power.end());
}

} // namespace rc::consumer
