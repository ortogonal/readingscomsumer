#pragma once

#include <stdint.h>

#include "consumer.h"

#include <sw/redis++/redis++.h>

namespace rc::consumer {

class TimeSeriesPower : public Consumer<TimeSeriesPower>
{
public:
    TimeSeriesPower(sw::redis::Redis &redis);
    void execute(const Json::Value &message);

    sw::redis::Redis& m_redis;

private:
    uint64_t toEpochTime(const std::string &str);

    void setPower(const std::string &key, const uint64_t epochTime, const Json::Value &powerJsonMap);
};

} // namespace rc::consumer
