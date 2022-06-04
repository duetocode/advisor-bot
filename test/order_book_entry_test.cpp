#include <gtest/gtest.h>
#include <order_book_entry.h>

TEST(OrderBookTypeTest, TestOperatorEQWithString)
{
    ASSERT_EQ(OrderBookType::bid, "bid");
    ASSERT_EQ(OrderBookType::ask, "ask");
    ASSERT_FALSE(OrderBookType::bid == "ask");
    ASSERT_NE(OrderBookType::ask, "bid");
}