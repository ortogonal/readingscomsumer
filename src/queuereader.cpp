#include "queuereader.h"

#include <iostream>

namespace rc {

QueueReader::QueueReader(sw::redis::Redis &redis)
    : m_redis(redis)
{}

void QueueReader::onNewMessage(QueueReaderMessageCallback &&cb)
{
    m_messageCallbacks.push_back(std::move(cb));
}

void QueueReader::start()
{
    m_running = true;
    const sw::redis::StringView key = "readings_queue";

    while (m_running) {
        auto msg = m_redis.brpop(key);
        if (msg.has_value()) {
            for (const auto &cb : m_messageCallbacks) {
                cb(msg.value().second);
            }
        }
    }
}

void QueueReader::stop()
{
    m_running = false;
}

} // namespace rc
