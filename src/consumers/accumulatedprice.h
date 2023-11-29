#pragma once

#include <sw/redis++/redis++.h>

#include "consumer.h"

#include "../providers/hour.h"
#include "../providers/idayaheadprice.h"

namespace rc::consumer {

class AccumulatedPrice : public Consumer<AccumulatedPrice>
{
public:
    AccumulatedPrice(sw::redis::Redis &redis, provider::IDayAheadPrice &dayAheadProvider);

    void execute(const Json::Value &message);

private:
    provider::Hour toHour(const std::string ts) const;
    void accumulatePrice(const std::string &key, const float &price, const Json::Value &powerMap);

    sw::redis::Redis &m_redis;
    provider::IDayAheadPrice &m_dayAheadProvider;
};

} // namespace rc::consumer
