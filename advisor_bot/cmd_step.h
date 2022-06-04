#pragma once

#include "advisor_bot.h"

class StepCommand : public AdvisorCommand
{
public:
    StepCommand();
    void execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput) override;
};