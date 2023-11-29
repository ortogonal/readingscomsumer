#pragma once

#include <functional>

#include <stdint.h>

namespace rc::provider {

struct Hour
{
    int year;
    int month;
    int day;
    int hour;

    bool operator==(const Hour &other) const
    {
        return year == other.year && month == other.month && day == other.day && hour == other.hour;
    }

    bool operator!=(const Hour &other) const { return !(*this == other); }

    int operator<(const Hour &other) const
    {
        if (year > other.year)
            return -1;
        else if (year < other.year)
            return 1;

        if (month > other.month)
            return -1;
        else if (month < other.month)
            return 1;

        if (day > other.day)
            return -1;
        else if (day < other.day)
            return 1;

        if (hour > other.hour)
            return -1;
        else if (hour < other.hour)
            return 1;

        return 0;
    }
};

} // namespace rc::provider

template<>
struct std::hash<rc::provider::Hour>
{
    std::size_t operator()(const rc::provider::Hour &k) const
    {
        using std::hash;
        using std::size_t;

        uint32_t seed = 0;
        seed ^= k.year + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= k.month + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= k.day + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= k.hour + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};
