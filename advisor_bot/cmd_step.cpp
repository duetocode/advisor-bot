#include "cmd_step.h"
#include <string>
#include <sstream>

StepCommand::StepCommand() : AdvisorCommand("step", CommandDescription{"move to next time step", "step"}) {}

void StepCommand::execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput)
{
    advisorBot.it++;
    if (advisorBot.it == advisorBot.getOrderBook().end())
    {
        advisorBot << "No more data available. Wraping back to the beginning" << std::endl;
        advisorBot.it = advisorBot.getOrderBook().begin();
    }

    advisorBot << "now at " << advisorBot.it->timestamp() << std::endl;
}