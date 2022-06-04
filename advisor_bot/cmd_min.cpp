#include "cmd_min.h"
#include <string>
#include <sstream>
#include "utils.h"

MinCommand::MinCommand() : AdvisorCommand("min", CommandDescription{"return the minimum of the speicified product in the current time step.", "min <product> <ask/bid>"}) {}

void MinCommand::execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput)
{
    // check the argument
    if (userInput.size() != 3 || (userInput[2] != "bid" && userInput[2] != "ask"))
    {
        advisorBot << "Invalid arguments. Usage: " << descriptions.begin()->second.usage << std::endl;
        return;
    }

    // Query
    auto entries = advisorBot.getOrderBook().getCurrentEntries();

    bool found = false;
    double candidate = 0.0;
    if (!entries.empty())
    {
        for (const auto &entry : entries)
        {
            if (entry.product != userInput[1] || entry.orderType != userInput[2])
            {
                continue;
            }

            if (!found || entry.price < candidate)
            {
                candidate = entry.price;
                found = true;
            }
        }
    }

    // Output
    if (!found)
    {
        advisorBot << "No avialbe records found in current time step." << std::endl;
    }
    else
    {
        advisorBot << "The minimum " << userInput[2] << " price for " << userInput[1] << " is " << format(candidate) << std::endl;
    }
}