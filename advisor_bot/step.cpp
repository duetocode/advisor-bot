#include "step.h"
#include <string>
#include <map>
#include <iostream>
#include <iomanip>

Step::Step() {}
Step::Step(std::vector<OrderBookEntry> _orders) : orders(_orders) {}
Step::Step(const Step &other) : orders(other.orders) {}
Step::Step(Step &&other)
{
    std::swap(orders, other.orders);
}

// copy assignment operator
Step &Step::operator=(const Step &other)
{
    orders = other.orders;
    return *this;
}
// move assignment operator
Step &Step::operator=(Step &&other)
{
    std::swap(orders, other.orders);
    return *this;
}

std::string Step::timestamp() const
{
    if (orders.size() > 0)
    {
        return orders[0].timestamp;
    }
    else
    {
        return "";
    }
}

std::vector<OrderBookEntry> Step::matchAsksToBids(std::string product)
{
    // sort entries by type
    std::vector<OrderBookEntry> asks, bids;
    for (auto &entry : orders)
    {
        if (entry.product != product)
            continue;

        if (entry.orderType == OrderBookType::ask)
        {
            asks.push_back(entry);
        }
        else if (entry.orderType == OrderBookType::bid)
        {
            bids.push_back(entry);
        }
    }

    // the result of the matching
    std::vector<OrderBookEntry> sales;

    // I put in a little check to ensure we have bids and asks
    // to process.
    if (asks.size() == 0 || bids.size() == 0)
    {
        std::cout << "no data" << std::endl;
        return sales;
    }

    // sort asks lowest first
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    // sort bids highest first
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);

    /*
    std::cout << "max ask " << asks[asks.size() - 1].price << std::endl;
    std::cout << "min ask " << asks[0].price << std::endl;
    std::cout << "max bid " << bids[0].price << std::endl;
    std::cout << "min bid " << bids[bids.size() - 1].price << std::endl;
    */

    for (OrderBookEntry &ask : asks)
    {
        //     for bid in bids:
        for (OrderBookEntry &bid : bids)
        {
            // the bid is lower than the ask, we can end the operation early since the bids and asks are all sorted
            if (bid.price - ask.price < -1e-6)
                break;

            // copy the ask entry as the matched order, but reset the amount to zero
            OrderBookEntry sale{ask};
            sale.amount = 0;

            // we do not need to address the entries placed by simuser since this is advisor bot.

            // the ask is match with the bid and there are enough funds to match the ask
            if (bid.amount >= ask.amount)
            {
                sale.amount = ask.amount;
                bid.amount -= ask.amount;
                ask.amount = 0;
                std::cout << "Matched: " << sale.product << " - " << sale.amount << std::endl;
                sales.push_back(sale);
            }
            // the ask is match with the bid and there are not enough funds
            else if (bid.amount < ask.amount &&
                     bid.amount > 0)
            {
                sale.amount = bid.amount;
                ask.amount -= bid.amount;
                bid.amount = 0;
                std::cout << "Matched: " << sale.product << " - " << sale.amount << std::endl;
                sales.push_back(sale);
            }
        }
    }
    return sales;
}

std::map<std::string, Stats> Step::stats() const
{
    std::map<std::string, Stats> result;

    for (auto &order : orders)
    {
        auto &stat = result[order.product];

        // numAsks and numBids
        if (order.orderType == OrderBookType::ask)
        {
            stat.numAsks++;
        }
        else if (order.orderType == OrderBookType::bid)
        {
            stat.numBids++;
        }
    }

    return result;
}