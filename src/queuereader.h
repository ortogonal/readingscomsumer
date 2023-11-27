#pragma once

#include <functional>
#include <string>
#include <vector>

#include <sw/redis++/redis++.h>

namespace rc {

class QueueReader {
public:
    using QueueReaderMessageCallback = std::function<void(const std::string&)>;

    QueueReader(sw::redis::Redis &redis);

    void onNewMessage(QueueReaderMessageCallback &&cb);

    void start();
    void stop();

private:
    std::vector<QueueReaderMessageCallback> m_messageCallbacks;

    sw::redis::Redis& m_redis;
    bool m_running = false;
};

}
