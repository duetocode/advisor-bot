#include "order_book.h"
#include "csv_reader.h"
#include <set>
#include <iostream>

OrderBook::OrderBook() {}

OrderBook::OrderBook(std::string csvFileName)
{
    orders = CSVReader::readCSV(csvFileName);

    // build steps
    std::string ts = "";
    for (auto &entry : orders)
    {
        if (ts != entry.timestamp)
        {
            steps.push_back(Step{});
            ts = entry.timestamp;
        }
        steps.back().orders.push_back(entry);
    }

    stepIter = steps.begin();
}

OrderBook::OrderBook(OrderBook &&other)
{
    std::swap(orders, other.orders);
    std::swap(steps, other.steps);
    std::swap(stepIter, other.stepIter);
}

OrderBook::OrderBook(OrderBook &other)
{
    orders = other.orders;
    steps = other.steps;
    stepIter = other.stepIter;
}

// move assignment operator
OrderBook &OrderBook::operator=(OrderBook &&other)
{
    std::swap(orders, other.orders);
    std::swap(steps, other.steps);
    std::swap(stepIter, other.stepIter);
    return *this;
}

// copy assignment operator
OrderBook &OrderBook::operator=(OrderBook &other)
{
    orders = other.orders;
    steps = other.steps;
    stepIter = other.stepIter;
    return *this;
}

size_t OrderBook::size() const
{
    return orders.size();
}

std::string OrderBook::getTimestamp() const
{
    return stepIter->timestamp();
}

std::string OrderBook::step()
{
    stepIter++;

    if (stepIter == steps.end())
    {
        stepIter = steps.begin();
    }

    return getTimestamp();
}

std::vector<OrderBookEntry> OrderBook::getCurrentEntries() const
{
    return stepIter->orders;
}

std::vector<std::string> OrderBook::getAvailableProducts() const
{
    // Use set to remove duplicates
    std::set<std::string> products;

    for (auto &order : orders)
    {
        products.insert(order.product);
    }

    return std::vector<std::string>(products.begin(), products.end());
}

Step &OrderBook::currentStep()
{
    return *stepIter;
}

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