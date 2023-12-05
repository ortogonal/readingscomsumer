#include "dayaheadprice.h"

#include <algorithm>
#include <iostream>

#include <cppconn/resultset.h>
#include <cppconn/statement.h>

namespace rc::provider {

DayAheadPrice::DayAheadPrice()
    : m_driver(get_driver_instance())
{
    auto host = std::getenv("DB_HOST");
    auto db = std::getenv("DB_DB");
    auto pass = std::getenv("DB_PASSWORD");

    m_connection = std::unique_ptr<sql::Connection>(m_driver->connect(host, db, pass));

    m_connection->setSchema("el");
    load();
}

float DayAheadPrice::priceAt(const Hour &hour) const
{
    if (m_prices.contains(hour)) {
        return m_prices.at(hour);
    }

    std::cerr << "Price not found" << std::endl;

    return 0.f;
}

void DayAheadPrice::load()
{
    const auto now = std::chrono::system_clock::now();
    const auto from = now - std::chrono::days(1);

    load(from);
}

void DayAheadPrice::load(const timepoint &from)
{
    auto toString = [](auto number) {
        auto str = std::to_string(number);

        auto extra = 2 - std::min(2, static_cast<int>(str.length()));
        str.insert(0, extra, '0');
        return str;
    };

    const std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(from)};
    std::cout << static_cast<int>(ymd.year()) << " " << static_cast<unsigned>(ymd.month()) << " "
              << static_cast<unsigned>(ymd.day()) << " " << static_cast<bool>(ymd.ok())
              << std::endl;

    auto statment = std::unique_ptr<sql::Statement>(m_connection->createStatement());
    auto query = "SELECT start, local_price FROM dayaheadprices WHERE start > "
                 + std::to_string(static_cast<int>(ymd.year()))
                 + toString(static_cast<unsigned>(ymd.month()))
                 + toString(static_cast<unsigned>(ymd.day())) + " LIMIT 48";
    std::cout << query << std::endl;
    auto res = std::unique_ptr<sql::ResultSet>(statment->executeQuery(query));

    while (res->next()) {
        //std::cout << res->getString(2) << std::endl;
        store(res->getString(1), res->getString(2));
    }
}

void DayAheadPrice::store(const std::string &startTime, const std::string &priceStr)
{
    if (startTime.size() > 13) {
        auto year = std::stoi(startTime.substr(0, 4));
        auto month = std::stoi(startTime.substr(5, 2));
        auto day = std::stoi(startTime.substr(8, 2));
        auto hour = std::stoi(startTime.substr(11, 2));
        auto price = std::stof(priceStr);

        Hour h = {year, month, day, hour};
        m_prices.insert(std::map<Hour, float>::value_type(h, price));
    }
}

} // namespace rc::provider
