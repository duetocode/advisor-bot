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

    it = steps.begin();
}

OrderBook::OrderBook(OrderBook &&other)
{
    std::swap(orders, other.orders);
    std::swap(steps, other.steps);
    std::swap(it, other.it);
}

OrderBook::OrderBook(OrderBook &other)
{
    orders = other.orders;
    steps = other.steps;
    it = other.it;
}

// move assignment operator
OrderBook &OrderBook::operator=(OrderBook &&other)
{
    std::swap(orders, other.orders);
    std::swap(steps, other.steps);
    std::swap(it, other.it);
    return *this;
}

// copy assignment operator
OrderBook &OrderBook::operator=(OrderBook &other)
{
    orders = other.orders;
    steps = other.steps;
    it = other.it;
    return *this;
}

size_t OrderBook::recordSize() const
{
    return orders.size();
}

size_t OrderBook::stepSize() const
{
    return steps.size();
}

Step &OrderBook::step()
{
    it++;

    // wrap to the beginning if we reached the end
    if (it == steps.end())
    {
        it = steps.begin();
    }

    return *it;
}

std::vector<Step *> OrderBook::getPreviousSteps(unsigned int num)
{
    // copy the iterator
    auto it = this->it;
    bool reached = false;
    std::vector<Step *> result;

    while (!reached && num != 0)
    {
        --num;
        result.push_back(&(*it));
        if (it == steps.begin())
            // reached the beginning, set the flag to break the loop
            reached = true;
        else
            --it;
    }

    return result;
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
    return *it;
}
