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

    /**
     * @brief parse OrderBookType from string
     *
     * @param str String representation of the OrderBookType
     * @return OrderBookType parsed from string
     */
    static OrderBookType stringToOrderBookType(std::string str);
};