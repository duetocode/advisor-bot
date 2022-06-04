#pragma once

#include "advisor_bot.h"

class AvgCommand : public AdvisorCommand
{
public:
    AvgCommand();
    void execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput) override;
};