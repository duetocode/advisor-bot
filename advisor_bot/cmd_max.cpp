#include "cmd_max.h"
#include <string>
#include <sstream>
#include "utils.h"

MaxCommand::MaxCommand() : AdvisorCommand(
                               "max", CommandDescription{
                                          "compute maximum ask or bid price in the current time step",
                                          "max <product> <ask/bid>  Example: max BTC/USDT bid"})
{
}

void MaxCommand::execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput)
{
    // check the argument
    if (userInput.size() != 3 || (userInput[2] != "bid" && userInput[2] != "ask"))
    {
        advisorBot << "Invalid arguments. Usage: " << descriptions.begin()->second.usage << std::endl;
        return;
    }

    double candidate = -1.0;
    for (const auto &entry : advisorBot.it->orders)
    {
        if (entry.product != userInput[1] || entry.orderType != userInput[2])
        {
            continue;
        }

        if (entry.price > candidate)
        {
            candidate = entry.price;
        }
    }

    // Output
    if (candidate < 0.0)
    {
        advisorBot << "No avialbe records found in current time step." << std::endl;
    }
    else
    {
        advisorBot << "The maximum " << userInput[2] << " price for " << userInput[1] << " is " << format(candidate) << std::endl;
    }
}
