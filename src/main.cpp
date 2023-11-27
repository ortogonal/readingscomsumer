#include <iostream>

#include <variant>
#include <vector>

#include <jsoncpp/json/json.h>

#include "queuereader.h"

#include "consumers/accumulatedpower.h"
#include "consumers/accumulatedprice.h"
#include "consumers/livepower.h"

#include "parsers/jsonmessage.h"

using namespace std;
using namespace rc;

using Consumers
    = variant<consumer::AccumulatedPower, consumer::AccumulatedPrice, consumer::LivePower>;

int main()
{
    auto redis = sw::redis::Redis("tcp://localhost:6379");

    QueueReader qr(redis);

    vector<Consumers> consumers{consumer::AccumulatedPower{redis} /*, consumer::AccumulatedPrice{}*/,
                                consumer::LivePower{redis}};

    qr.onNewMessage([&consumers](const std::string &message) {
        auto msg = parser::toJSONMessage(message);
        if (msg.has_value()) {
            auto root = msg.value();

            for (auto &c : consumers) {
                std::visit([root](auto &c) { consumer::execute(c, root); }, c);
            }
        } else {
            std::cerr << "Parsing error" << std::endl;
        }
    });

    qr.start();

    return 0;
}
