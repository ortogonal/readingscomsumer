#pragma once

#include "consumer.h"

namespace rc::consumer {

class AccumulatedPrice : public Consumer<AccumulatedPrice>
{
public:
    void execute(const Json::Value &message);
};

} // namespace rc::consumer
