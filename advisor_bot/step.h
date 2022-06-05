#pragma once

#include <string>
#include <vector>
#include <map>
#include "order_book_entry.h"

/**
 * @brief Class represents a single time step in the simulation.
 *
 */
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

    /* match asks and bids in the current time frame */
    std::vector<OrderBookEntry> matchAsksToBids(std::string product);
};
