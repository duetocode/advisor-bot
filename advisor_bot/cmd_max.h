#pragma once

#include "advisor_bot.h"

class MaxCommand : public AdvisorCommand
{
public:
    MaxCommand();
    void execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput) override;
};