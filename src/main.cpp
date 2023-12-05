#include <iostream>

#include <variant>
#include <vector>

#include <jsoncpp/json/json.h>

#include "queuereader.h"

#include "consumers/accumulatedpower.h"
#include "consumers/accumulatedprice.h"
#include "consumers/livepower.h"
#include "consumers/power.h"

#include "parsers/jsonmessage.h"

#include "providers/dayaheadprice.h"

using namespace std;
using namespace rc;

using Consumers = variant<consumer::AccumulatedPower,
                          consumer::AccumulatedPrice,
                          consumer::LivePower,
                          consumer::TimeSeriesPower>;

int main()
{
    auto redis = sw::redis::Redis("tcp://192.168.1.27:6379");
    QueueReader qr(redis);

    provider::DayAheadPrice dayAheadPrice;

    vector<Consumers> consumers{consumer::AccumulatedPower{redis},
                                consumer::AccumulatedPrice{redis, dayAheadPrice},
                                consumer::LivePower{redis},
                                consumer::TimeSeriesPower{redis}};

    qr.onNewMessage([&consumers](const std::string &message) {
        //std::cout << message << std::endl;
        auto msg = parser::toJSONMessage(message);
        if (msg.has_value()) {
            auto rootElement = msg.value();

            for (auto &consumer : consumers) {
                std::visit([rootElement](auto &c) { consumer::execute(c, rootElement); }, consumer);
            }
        } else {
            std::cerr << "Parsing error" << std::endl;
        }
    });

    qr.start();

    return 0;
}
