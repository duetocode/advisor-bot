#include <gtest/gtest.h>
#include <order_book_entry.h>

TEST(OrderBookTypeTest, TestOperatorEQWithString)
{
    ASSERT_EQ(OrderBookType::bid, "bid");
    ASSERT_EQ(OrderBookType::ask, "ask");
    ASSERT_FALSE(OrderBookType::bid == "ask");
    ASSERT_NE(OrderBookType::ask, "bid");
}

TEST(OrderBookEntryTest, TestMapAutoInitialization)
{
    std::map<std::string, std::vector<int>> table;
    table["a"].push_back(1);

    ASSERT_EQ(table.count("a"), 1);
    ASSERT_EQ(table.at("a").size(), 1);
    ASSERT_EQ(table.at("a")[0], 1);
}