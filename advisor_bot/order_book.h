#pragma once

#include <vector>
#include <string>
#include "order_book_entry.h"
#include "step.h"

class OrderBook
{
public:
    // container holding oall orders
    std::vector<OrderBookEntry> orders;
    std::vector<Step> steps;

    OrderBook();
    OrderBook(std::string csvFileName);

    // copy constructor
    OrderBook(OrderBook &other);
    // move constructor
    OrderBook(OrderBook &&other);

    // move assignment
    OrderBook &operator=(OrderBook &&other);
    // copy assignment
    OrderBook &operator=(OrderBook &other);

    /* go the the next step and return the new timestamp */
    Step &step();
    /* get current step of the simulation */
    Step &currentStep();
    /* get previous step */
    std::vector<Step *> getPreviousSteps(unsigned int num);

    /* get the number of entries in the order book */
    size_t recordSize() const;
    size_t stepSize() const;

    /* get list of availble products */
    std::vector<std::string> getAvailableProducts() const;

private:
    std::vector<Step>::iterator it;
};