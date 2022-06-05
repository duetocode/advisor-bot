#include "cmd_stat.h"
#include <string>
#include <sstream>
#include "utils.h"

StatCommand::StatCommand() : AdvisorCommand("stat", CommandDescription{"Show statisitics information about current time frame", "stat"}) {}

void StatCommand::execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput)
{
    // check the argument
    if (userInput.size() != 1)
    {
        advisorBot << "Invalid arguments. Usage: " << descriptions.begin()->second.usage << std::endl;
        return;
    }

    auto stats = advisorBot.getOrderBook().currentStep().stats();

    if (stats.empty())
    {
        advisorBot << "No data avaible in current step." << std::endl;
    }
    else
    {
        advisorBot << "Current step statistics:" << std::endl;
        for (const auto &[key, value] : stats)
        {
            advisorBot << key << ":" << std::endl;
            advisorBot << "\t"
                       << "asks: " << value.numAsks << std::endl;
            advisorBot << "\t"
                       << "bids: " << value.numBids << std::endl;
        }
    }
}