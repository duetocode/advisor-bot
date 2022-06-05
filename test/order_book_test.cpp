#include <gtest/gtest.h>
#include "order_book.h"
#include <filesystem>
#include <cmath>

TEST(OrderBookTest, LoadFromCSV)
{
    OrderBook orderBook("test_data.csv");
    ASSERT_EQ(orderBook.recordSize(), 500);
    ASSERT_EQ(orderBook.steps.size(), 2);

    ASSERT_EQ(orderBook.steps[0].orders.size(), 476);
    ASSERT_EQ(orderBook.steps[0].orders[0].timestamp, "2020/06/01 11:57:30.328127");
    ASSERT_EQ(orderBook.steps[1].orders.size(), 24);
    ASSERT_EQ(orderBook.steps[1].orders[0].timestamp, "2020/06/01 11:57:35.334211");
}

TEST(OrderBookTest, TestCopy)
{
    OrderBook orderBook("test_data.csv");
    ASSERT_EQ(orderBook.recordSize(), 500);

    OrderBook orderBook2 = orderBook;
    ASSERT_EQ(orderBook.recordSize(), 500);
    ASSERT_EQ(orderBook2.recordSize(), 500);
}

TEST(OrderBookTest, TestOrderBookMove)
{
    OrderBook orderBook{"test_data.csv"};
    ASSERT_EQ(orderBook.recordSize(), 500);
    ASSERT_EQ(orderBook.currentStep().timestamp(), "2020/06/01 11:57:30.328127");

    OrderBook orderBook2 = std::move(orderBook);
    ASSERT_EQ(orderBook2.recordSize(), 500);
    ASSERT_EQ(orderBook.recordSize(), 0);
    ASSERT_EQ(orderBook2.currentStep().timestamp(), "2020/06/01 11:57:30.328127");
    // Cannot access getTimestamp since it is in an invalid state
}

TEST(OrderBookTest, FirstTimestamp)
{
    OrderBook orderBook{"test_data.csv"};
    ASSERT_EQ(orderBook.currentStep().timestamp(), "2020/06/01 11:57:30.328127");
}

TEST(OrderBookTest, TestStep)
{
    OrderBook orderBook{"test_data.csv"};
    ASSERT_EQ(orderBook.recordSize(), 500);

    Step &actual = orderBook.step();
    ASSERT_EQ(actual.timestamp(), "2020/06/01 11:57:35.334211");

    // should wrap back to the beginning if we reach the end
    actual = orderBook.step();
    ASSERT_EQ(actual.timestamp(), "2020/06/01 11:57:30.328127");
}

TEST(OrderBookTest, TestGetCurrentEntries)
{
    OrderBook orderBook{"test_data.csv"};
    ASSERT_EQ(orderBook.recordSize(), 500);

    std::vector<OrderBookEntry> actual = orderBook.currentStep().orders;
    ASSERT_EQ(actual.size(), 476);
    ASSERT_EQ(actual[0].timestamp, "2020/06/01 11:57:30.328127");
    ASSERT_EQ(actual.back().timestamp, actual[0].timestamp);
}

TEST(OrderBookTest, TestGetAvailableProducts)
{
    OrderBook orderBook{"test_data.csv"};
    ASSERT_EQ(orderBook.recordSize(), 500);

    const std::vector<std::string> expected = {"BTC/USDT", "DOGE/BTC", "DOGE/USDT", "ETH/BTC", "ETH/USDT"};
    auto actual = orderBook.getAvailableProducts();
    std::sort(actual.begin(), actual.end());

    ASSERT_EQ(actual, expected);
}

TEST(OrderBookIteratorTest, TestIteratorExp)
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    auto it = v.begin();
    it += 2;
    ASSERT_EQ(*it, 3);

    bool reached = false;
    int expected = 3;
    while (!reached)
    {
        ASSERT_EQ(*it, expected);
        --expected;
        if (it == v.begin())
            reached = true;
        else
            --it;
    }
    ASSERT_EQ(it, v.begin());
    ASSERT_EQ(*it, 1);
}

TEST(OrderBookIteratorTest, TestGetPreviousSteps)
{
    OrderBook orderBook{"test_data.csv"};
    ASSERT_EQ(orderBook.recordSize(), 500);

    // current step should be included
    auto result = orderBook.getPreviousSteps(1);
    ASSERT_EQ(result.size(), 1);

    orderBook.step();

    result = orderBook.getPreviousSteps(1);
    ASSERT_EQ(result.size(), 1);
    result = orderBook.getPreviousSteps(2);
    ASSERT_EQ(result.size(), 2);

    result = orderBook.getPreviousSteps(200);
    ASSERT_EQ(result.size(), 2);
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
    ASSERT_EQ(orderBook.recordSize(), 500);

    orderBook.step();

    auto actual = orderBook.currentStep().matchAsksToBids("BTC/USDT");

    ASSERT_EQ(actual.size(), 0);

    orderBook.step();
    actual = orderBook.currentStep().matchAsksToBids("BTC/USDT");
    ASSERT_EQ(actual.size(), 1);
    ASSERT_EQ(actual[0].amount, 1.51142778);

    // the matching operation should be idempotent
    actual = orderBook.currentStep().matchAsksToBids("BTC/USDT");
    ASSERT_EQ(actual.size(), 1);
    ASSERT_EQ(actual[0].amount, 1.51142778);
}