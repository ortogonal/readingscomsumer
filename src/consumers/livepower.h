#pragma once

#include "consumer.h"

#include <sw/redis++/redis++.h>

namespace rc::consumer {

class LivePower : public Consumer<LivePower>
{
public:
    LivePower(sw::redis::Redis &redis);
    void execute(const Json::Value &message);

private:
    void setLivePower(const std::string &key, const Json::Value &powerJsonMap);

    sw::redis::Redis& m_redis;
};

} // namespace rc::consumer
