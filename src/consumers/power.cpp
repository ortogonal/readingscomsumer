#include "power.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>

namespace rc::consumer {

TimeSeriesPower::TimeSeriesPower(sw::redis::Redis &redis)
    : m_redis(redis)
{}

void TimeSeriesPower::execute(const Json::Value &message)
{
    if (message["type"] == "han") {
        auto data = message["data"];

        auto id = data["id"].asString();
        std::transform(id.begin(), id.end(), id.begin(), [](char ch) {
            return ch == ' ' ? '_' : ch;
        });

        auto ts = data["timestamp"].asString();
        uint64_t epochTS = toEpochTime(ts);

        setPower("ts:power:in:" + id, epochTS, data["input"]);
        setPower("ts:power:out:" + id, epochTS, data["output"]);
    }
}

void TimeSeriesPower::setPower(const std::string &key,
                               const uint64_t epochTime,
                               const Json::Value &powerJsonMap)
{
    std::vector<sw::redis::OptionalLongLong> sorted_list;

    try {
        auto r = m_redis.command(std::string("TS.ADD"),
                                 key,
                                 std::to_string(epochTime * 1000),
                                 powerJsonMap["total"].asString());
        auto num = sw::redis::reply::parse<long long>(*r);
    } catch(sw::redis::ParseError error) {
        std::cout << "Error " << error.what() << std::endl;
        exit(-1);
    }
}

uint64_t TimeSeriesPower::toEpochTime(const std::string &str)
{
    auto year = 2000 + std::stoi(str.substr(0, 2));
    auto month = std::stoi(str.substr(2, 2));
    auto day = std::stoi(str.substr(4, 2));
    auto hour = std::stoi(str.substr(6, 2));
    auto min = std::stoi(str.substr(8, 2));
    auto sec = std::stoi(str.substr(10, 2));

    auto date = std::chrono::year_month_day{std::chrono::year(year),
                                            std::chrono::month(month),
                                            std::chrono::day(day)};

    auto sysDate = std::chrono::sys_days{date};
    auto epoch = std::chrono::sys_days{date}.time_since_epoch().count() * 24 * 3600;
    epoch += hour * 3600;
    epoch += min * 60;
    epoch += sec;

    return epoch;
}

} // namespace rc::consumer
