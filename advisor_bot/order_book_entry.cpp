#include "order_book_entry.h"

bool operator==(const OrderBookType &lhs, const std::string &rhs)
{
    if (lhs == OrderBookType::ask && rhs == "ask")
        return true;
    else if (lhs == OrderBookType::bid && rhs == "bid")
        return true;
    else
        return false;
}
bool operator!=(const OrderBookType &lhs, const std::string &rhs)
{
    return !(lhs == rhs);
}

OrderBookEntry::OrderBookEntry(double _price,
                               double _amount,
                               std::string _timestamp,
                               std::string _product,
                               OrderBookType _orderType)
    : price(_price),
      amount(_amount),
      timestamp(_timestamp),
      product(_product),
      orderType(_orderType)
{
}

OrderBookType OrderBookEntry::stringToOrderBookType(std::string s)
{
    if (s == "ask")
    {
        return OrderBookType::ask;
    }
    else if (s == "bid")
    {
        return OrderBookType::bid;
    }
    else
    {
        return OrderBookType::unknown;
    }
}
