#pragma once

#include <string>

enum class OrderBookType
{
    bid,
    ask,
    unknown,
};

bool operator==(const OrderBookType &lhs, const std::string &rhs);
bool operator!=(const OrderBookType &lhs, const std::string &rhs);

/** Represents a single entry in the order book. */
class OrderBookEntry
{
public:
    double price;
    double amount;
    std::string timestamp;
    std::string product;
    OrderBookType orderType;
    std::string username;

    OrderBookEntry(double _price,
                   double _amount,
                   std::string _timestamp,
                   std::string _product,
                   OrderBookType _orderType);
    // copy constructor
    OrderBookEntry(const OrderBookEntry &other);

    /**
     * @brief parse OrderBookType from string
     *
     * @param str String representation of the OrderBookType
     * @return OrderBookType parsed from string
     */
    static OrderBookType stringToOrderBookType(std::string str);

    static bool compareByTimestamp(OrderBookEntry &e1, OrderBookEntry &e2)
    {
        return e1.timestamp < e2.timestamp;
    }
    static bool compareByPriceAsc(OrderBookEntry &e1, OrderBookEntry &e2)
    {
        return e1.price < e2.price;
    }
    static bool compareByPriceDesc(OrderBookEntry &e1, OrderBookEntry &e2)
    {
        return e1.price > e2.price;
    }
};