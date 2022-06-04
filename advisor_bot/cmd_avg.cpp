#include "cmd_avg.h"
#include <string>
#include <sstream>
#include "utils.h"
#include "order_book_entry.h"

AvgCommand::AvgCommand() : AdvisorCommand(
                               "avg", CommandDescription{
                                          "compute the average ask or bid price in the specified time steps",
                                          "avg <product> <ask/bid> <time steps>"}) {}

int parseTimestepsArg(std::string str)
{
    try
    {
        return std::stoi(str);
    }
    catch (std::exception)
    {
        return -1;
    }
}

void AvgCommand::execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput)
{
    int timesteps = -1;
    // check the argument
    if (userInput.size() != 4 || (userInput[2] != "bid" && userInput[2] != "ask") || (timesteps = parseTimestepsArg(userInput[3])) == -1)
    {
        advisorBot << "Invalid arguments. Usage: " << descriptions.begin()->second.usage << std::endl;
        return;
    }

    auto product = userInput[1];
    auto type = userInput[2] == "bid" ? OrderBookType::bid : OrderBookType::ask;

    auto &orderBook = advisorBot.getOrderBook();
    auto &step = orderBook.currentStep();
    auto it = orderBook.steps.rbegin();

    // skip to current step
    while (it != orderBook.steps.rend() && it->timestamp() != step.timestamp())
        ++it;

    // accumulate
    size_t count = 0, stepCounter = timesteps;
    double sum = 0;
    while (timesteps != 0 && it != orderBook.steps.rend())
    {
        for (auto &order : it->orders)
        {
            if (order.product == product && order.orderType == type)
            {
                sum += order.price;
                count++;
            }
        }
        --stepCounter;
        ++it;
    }

    if (count == 0)
    {
        advisorBot << "No data found" << std::endl;
    }
    else
    {
        double result = sum / count;
        advisorBot << "The average " << product << " ask price over the last " << timesteps << " timesteps was " << format(result) << std::endl;
    }
}