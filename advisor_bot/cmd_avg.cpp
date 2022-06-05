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

    // parse
    auto product = userInput[1];
    auto type = userInput[2] == "bid" ? OrderBookType::bid : OrderBookType::ask;

    // copy the iterator for backtracking
    auto it = advisorBot.it;
    auto steps = advisorBot.getOrderBook().getPreviousSteps(it, timesteps);

    // accumulate
    size_t count = 0;
    double sum = 0.0, amount = 0.0;
    // copy a new iterator
    for (const auto &step : steps)
    {
        for (auto &order : step.orders)
        {
            if (order.product == product && order.orderType == type)
            {
                sum += order.price * order.amount;
                amount += order.amount;
            }
        }
        count++;
    }

    // in case there is no data
    if (count == 0)
    {
        advisorBot << "No data found" << std::endl;
        return;
    }

    // calculate the average
    double result = sum / amount;
    advisorBot << "The average " << product << " ask price over the last " << timesteps << " timesteps was " << format(result) << std::endl;
}