#pragma once

#include <jsoncpp/json/json.h>

namespace rc::consumer {

template<typename Derived>
class Consumer
{
public:
    void interface(const Json::Value &message)
    {
        static_cast<Derived *>(this)->execute(message);
    }
    void execute(const Json::Value &message) {};
};

template<typename Base>
void execute(Base &base, const Json::Value &message)
{
    base.interface(message);
}

} // namespace rc::consumer
