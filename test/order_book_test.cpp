#include <gtest/gtest.h>
#include "order_book.h"
#include <filesystem>
#include <cmath>

TEST(OrderBookTest, TestInitialization)
{
    OrderBook ob("test_data.csv");
}

TEST(OrderBookTest, TestIterator)
{
    OrderBook ob{"test_data.csv"};
    StepIterator it = ob.begin();

    // 2020/06/01 11:57:30.328127,ETH/BTC,bid,0.02482205,23.9999428

    ASSERT_EQ(it->timestamp(), "2020/06/01 11:57:30.328127");
    ASSERT_EQ(it->orders.size(), 476);
    ASSERT_EQ(it->orders.back().timestamp, "2020/06/01 11:57:30.328127");
    ASSERT_EQ(it->orders[0].price, 0.02482205);
    ASSERT_EQ(it->orders.back().price, 0.002758);

    it++;
    ASSERT_EQ(it->timestamp(), "2020/06/01 11:57:35.334211");
    ASSERT_EQ(it->orders.back().timestamp, "2020/06/01 11:57:35.334211");
    ASSERT_EQ(it->orders[0].price, 0.02482736);
    ASSERT_EQ(it->orders.back().price, 0.02471974);

    ++it;
    ASSERT_EQ(it, ob.end());

    --it;
    ASSERT_NE(it, ob.end());

    --it;
    ASSERT_EQ(it->timestamp(), "2020/06/01 11:57:30.328127");
    ASSERT_EQ(it->orders.size(), 476);
    ASSERT_EQ(it->orders.back().timestamp, "2020/06/01 11:57:30.328127");
    ASSERT_EQ(it->orders[0].price, 0.02482205);
    ASSERT_EQ(it->orders.back().price, 0.002758);

    try
    {
        --it;
        FAIL() << "should throw out_of_range exception";
    }
    catch (std::out_of_range const &e)
    {
    }
}

TEST(OrderBookTest, FirstTimestamp)
{
    OrderBook orderBook{"test_data.csv"};
    auto it = orderBook.begin();
    ASSERT_EQ(it->timestamp(), "2020/06/01 11:57:30.328127");
}

TEST(OrderBookTest, TestGetCurrentEntries)
{
    OrderBook orderBook{"test_data.csv"};

    auto it = orderBook.begin();

    std::vector<OrderBookEntry> actual = it->orders;
    ASSERT_EQ(actual.size(), 476);
    ASSERT_EQ(actual[0].timestamp, "2020/06/01 11:57:30.328127");
    ASSERT_EQ(actual.back().timestamp, actual[0].timestamp);
}

TEST(OrderBookStep, TestFloatComparision)
{
    double a = 9541.66999094;
    double b = 9541.66999095;

    ASSERT_TRUE(b > a);

    ASSERT_EQ(std::round(a * 1e6), std::round(b * 1e6));
}

TEST(OrderBookStep, TestMatching)
{
    OrderBook orderBook{"test_data.csv"};

    auto it = orderBook.begin();

    auto actual = it->matchAsksToBids("BTC/USDT");
    ASSERT_EQ(actual.size(), 1);

    ++it;
    actual = it->matchAsksToBids("BTC/USDT");
    ASSERT_EQ(actual.size(), 1);
    ASSERT_EQ(actual[0].amount, 1.51142778);

    // the matching operation should be idempotent
    actual = it->matchAsksToBids("BTC/USDT");
    ASSERT_EQ(actual.size(), 1);
    ASSERT_EQ(actual[0].amount, 1.51142778);
}