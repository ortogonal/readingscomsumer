#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <accumulatedpower.h>
#include <sw/redis++/redis++.h>

#include <map>

using namespace testing;

static std::map<std::string, double> increments;
static double incrementValue = 0;

class IRedis
{
public:
    virtual ~IRedis() = default;
    virtual double incrbyfloat(const sw::redis::StringView &key, double increment) = 0;
};

class RedisMock : public IRedis
{
public:
    MOCK_METHOD(double,
                incrbyfloat,
                (const sw::redis::StringView &key, double increment),
                (override));
};

RedisMock redisMock;

namespace sw::redis {

double Redis::incrbyfloat(const StringView &key, double increment)
{
    //increments.insert({std::string(key), increment});
    //return incrementValue;
    return redisMock.incrbyfloat(key, increment);
}

} // namespace sw::redis

namespace rc::consumer::test {

/*TEST(tst_accumulatedpower, execute_empty_message)
{
    sw::redis::Redis *redis;
    AccumulatedPower consumer(*redis);

    Json::Value value;
    consumer.execute(value);

    EXPECT_TRUE(increments.empty());
}*/

TEST(tst_accumulatedpower, execute_invalid_messages_just_han_type)
{
    sw::redis::Redis *redis;
    AccumulatedPower consumer(*redis);

    std::string json;
    json = "{\"type\": \"han\" }";

    Json::Reader reader;
    Json::Value msg;
    EXPECT_TRUE(reader.parse(json, msg, false));

    EXPECT_CALL(redisMock, incrbyfloat(_, _)).Times(0);
    consumer.execute(msg);
    Mock::AllowLeak(&redisMock);
}

/*TEST(tst_accumulatedpower, execute_valid_input_invalid_outout)
{
    sw::redis::Redis *redis;
    AccumulatedPower consumer(*redis);

    std::string json;

    json = "{\"type\": \"han\", "
           "   \"data\":"
           "       { \"input\": {"
           "               \"total\": 720,"
           "               \"l1\": \"1\","
           "               \"l2\": \"2\","
           "               \"l3\": \"3\""
           "           }"
           "       }"
           "}";
    Json::Reader reader;
    Json::Value msg;
    EXPECT_TRUE(reader.parse(json, msg, false));
    EXPECT_CALL(redisMock, incrbyfloat("acc:power:in:", 1)).Times(1);
    consumer.execute(msg);
    Mock::AllowLeak(&redisMock);
}*/

} // namespace rc::consumer::test
