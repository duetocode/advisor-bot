#pragma once

#include "order_book_entry.h"
#include <vector>
#include <string>

class CSVReader
{
public:
    static std::vector<OrderBookEntry> readCSV(std::string csvFile);
    static std::vector<std::string> tokenise(std::string csvLine, char separator);

    static OrderBookEntry stringsToOBE(std::string price,
                                       std::string amount,
                                       std::string timestamp,
                                       std::string product,
                                       OrderBookType OrderBookType);

    static OrderBookEntry stringsToOBE(std::vector<std::string> strings);
};