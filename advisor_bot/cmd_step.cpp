#include "cmd_step.h"
#include <string>
#include <sstream>

StepCommand::StepCommand() : AdvisorCommand("step", CommandDescription{"move to next time step", "step"}) {}

void StepCommand::execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput)
{
    Step &step = advisorBot.getOrderBook().step();

    advisorBot << "now at " << step.timestamp() << std::endl;
}