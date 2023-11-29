#include <gtest/gtest.h>

#include <hour.h>

namespace rc::provider::test {

TEST(tst_hour, check_equal)
{
    Hour h1{2000, 10, 10, 10};
    Hour h2{2000, 10, 10, 10};
    EXPECT_EQ(h1, h2);

    Hour f1{2001, 10, 10, 10};
    Hour f2{2000, 11, 10, 10};
    Hour f3{2000, 10, 11, 10};
    Hour f4{2000, 10, 10, 11};

    EXPECT_NE(h1, f1);
    EXPECT_NE(h1, f2);
    EXPECT_NE(h1, f3);
    EXPECT_NE(h1, f4);
}

TEST(tst_hour, check_less_than)
{
    Hour a1{2000, 10, 10, 9};
    Hour b1{2000, 10, 10, 10};
    EXPECT_EQ(a1 < b1, true);

    Hour a2{2000, 10, 9, 10};
    Hour b2{2000, 10, 10, 10};
    EXPECT_EQ(a2 < b2, true);

    Hour a3{2000, 9, 10, 10};
    Hour b3{2000, 10, 10, 10};
    EXPECT_EQ(a3 < b3, true);

    Hour a4{1999, 10, 10, 10};
    Hour b4{2000, 10, 10, 10};
    EXPECT_EQ(a4 < b4, true);
}

} // namespace rc::provider::test
