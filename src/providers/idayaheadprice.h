#pragma once

#include "hour.h"

namespace rc::provider {

class IDayAheadPrice
{
public:
    virtual ~IDayAheadPrice() = default;

    [[nodiscard]] virtual float priceAt(const Hour &hour) const = 0;
};

} // namespace rc::provider
