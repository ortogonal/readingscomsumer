#pragma once

#include "consumer.h"

#include <sw/redis++/redis++.h>

namespace rc::consumer {

class AccumulatedPower : public Consumer<AccumulatedPower>
{
public:
    AccumulatedPower(sw::redis::Redis &redis);
    void execute(const Json::Value &message);

private:
    void accumulatePower(const std::string &key, const Json::Value &powerMap);

    sw::redis::Redis& m_redis;
};

} // namespace rc::consumer
