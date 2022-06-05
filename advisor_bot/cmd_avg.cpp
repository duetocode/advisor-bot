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

    std::vector<Step *> previousSteps = orderBook.getPreviousSteps(timesteps);

    // accumulate
    size_t count = 0;
    double sum = 0;
    for (auto *step : previousSteps)
    {
        for (auto &order : step->orders)
        {
            if (order.product == product && order.orderType == type)
            {
                sum += order.price;
                count++;
            }
        }
        ++count;
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