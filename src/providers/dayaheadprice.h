#pragma once

#include <chrono>
#include <memory>
#include <unordered_map>

#include <cppconn/driver.h>

#include "hour.h"
#include "idayaheadprice.h"

namespace rc::provider {

class DayAheadPrice : public IDayAheadPrice
{
public:
    using timepoint = std::chrono::time_point<std::chrono::system_clock>;

    DayAheadPrice();

    [[nodiscard]] float priceAt(const Hour &hour) const override;

private:
    void load();
    void load(const timepoint &from);

    void store(const std::string &startTime, const std::string &price);

    sql::Driver *m_driver;
    std::unique_ptr<sql::Connection> m_connection;

    std::unordered_map<Hour, float> m_prices;
};

} // namespace rc::provider
