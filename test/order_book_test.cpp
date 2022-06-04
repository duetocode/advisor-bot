#include <gtest/gtest.h>
#include "order_book.h"
#include <filesystem>

TEST(OrderBookTest, LoadFromCSV)
{
    OrderBook orderBook("test_data.csv");
    ASSERT_EQ(orderBook.size(), 500);
    ASSERT_EQ(orderBook.steps.size(), 2);

    ASSERT_EQ(orderBook.steps[0].orders.size(), 476);
    ASSERT_EQ(orderBook.steps[0].orders[0].timestamp, "2020/06/01 11:57:30.328127");
    ASSERT_EQ(orderBook.steps[1].orders.size(), 24);
    ASSERT_EQ(orderBook.steps[1].orders[0].timestamp, "2020/06/01 11:57:35.334211");
}

TEST(OrderBookTest, TestCopy)
{
    OrderBook orderBook("test_data.csv");
    ASSERT_EQ(orderBook.size(), 500);

    OrderBook orderBook2 = orderBook;
    ASSERT_EQ(orderBook.size(), 500);
    ASSERT_EQ(orderBook2.size(), 500);
}

TEST(OrderBookTest, TestOrderBookMove)
{
    OrderBook orderBook{"test_data.csv"};
    ASSERT_EQ(orderBook.size(), 500);
    ASSERT_EQ(orderBook.getTimestamp(), "2020/06/01 11:57:30.328127");

    OrderBook orderBook2 = std::move(orderBook);
    ASSERT_EQ(orderBook2.size(), 500);
    ASSERT_EQ(orderBook.size(), 0);
    ASSERT_EQ(orderBook2.getTimestamp(), "2020/06/01 11:57:30.328127");
    // Cannot access getTimestamp since it is in an invalid state
}

TEST(OrderBookTest, FirstTimestamp)
{
    OrderBook orderBook{"test_data.csv"};
    ASSERT_EQ(orderBook.getTimestamp(), "2020/06/01 11:57:30.328127");
}

TEST(OrderBookTest, TestStep)
{
    OrderBook orderBook{"test_data.csv"};
    ASSERT_EQ(orderBook.size(), 500);

    std::string actual = orderBook.step();
    ASSERT_EQ(actual, "2020/06/01 11:57:35.334211");

    // should wrap back to the beginning if we reach the end
    actual = orderBook.step();
    ASSERT_EQ(actual, "2020/06/01 11:57:30.328127");
}

TEST(OrderBookTest, TestGetCurrentEntries)
{
    OrderBook orderBook{"test_data.csv"};
    ASSERT_EQ(orderBook.size(), 500);

    std::vector<OrderBookEntry> actual = orderBook.getCurrentEntries();
    ASSERT_EQ(actual.size(), 476);
    ASSERT_EQ(actual[0].timestamp, "2020/06/01 11:57:30.328127");
    ASSERT_EQ(actual.back().timestamp, actual[0].timestamp);
}

TEST(OrderBookTest, TestGetAvailableProducts)
{
    OrderBook orderBook{"test_data.csv"};
    ASSERT_EQ(orderBook.size(), 500);

    const std::vector<std::string> expected = {"BTC/USDT", "DOGE/BTC", "DOGE/USDT", "ETH/BTC", "ETH/USDT"};
    auto actual = orderBook.getAvailableProducts();
    std::sort(actual.begin(), actual.end());

    ASSERT_EQ(actual, expected);
}

TEST(OrderBookIteratorTest, TestIterator)
{
    OrderBook orderBook{"test_data.csv"};
    ASSERT_EQ(orderBook.size(), 500);
}