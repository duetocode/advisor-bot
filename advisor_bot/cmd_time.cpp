#include "cmd_time.h"
#include <string>
#include <sstream>
#include "utils.h"

TimeCommand::TimeCommand() : AdvisorCommand("time", CommandDescription{"show the time of the frame we are at.", "time"}) {}

void TimeCommand::execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput)
{
    // check the argument
    if (userInput.size() != 1)
    {
        advisorBot << "Invalid arguments. Usage: " << descriptions.begin()->second.usage << std::endl;
        return;
    }

    advisorBot << advisorBot.getOrderBook().currentStep().timestamp() << std::endl;
}