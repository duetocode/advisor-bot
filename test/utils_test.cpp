#include <gtest/gtest.h>
#include "utils.h"

TEST(UtilsTest, TestDoubleFormat)
{
    const double value = 0.0000027;

    const auto actual = format(value);

    ASSERT_EQ(actual, "0.0000027");
}