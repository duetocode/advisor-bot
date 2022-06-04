#pragma once

#include <vector>
#include <string>
#include "order_book_entry.h"

class Step
{
public:
    std::vector<OrderBookEntry> orders;

    Step();
    Step(std::vector<OrderBookEntry> _orders);
    // copy constructor
    Step(const Step &other);
    // move constructor
    Step(Step &&other);

    // copy assignment operator
    Step &operator=(const Step &other);
    // move assignment operator
    Step &operator=(Step &&other);

    /** get current timestamp */
    std::string timestamp() const;
};

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

    /* get the timestamp of the current step */
    std::string getTimestamp() const;
    /* go the the next step and return the new timestamp */
    std::string step();
    /* get all the entries in current step */
    std::vector<OrderBookEntry> getCurrentEntries() const;

    /* get the number of entries in the order book */
    size_t size() const;

    /* get list of availble products */
    std::vector<std::string> getAvailableProducts() const;

    /* get the current step */
    Step &currentStep();

    std::vector<Step>::iterator stepIter;
};