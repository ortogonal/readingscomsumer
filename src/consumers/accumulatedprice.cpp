#include "accumulatedprice.h"

#include <iostream>

namespace rc::consumer {

void AccumulatedPrice::execute(const Json::Value &message)
{
    std::cout << "PRICE - Delta is: " << message << std::endl;
}

} // namespace rc::consumer
