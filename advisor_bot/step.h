#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include "order_book_entry.h"

struct Stats
{
    size_t numAsks;
    size_t numBids;
};

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
    /** make statisitics*/
    std::map<std::string, Stats> stats() const;

    /* match asks and bids in the current time frame */
    std::vector<OrderBookEntry> matchAsksToBids(std::string product);
};
