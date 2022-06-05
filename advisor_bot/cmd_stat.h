#pragma once

#include "advisor_bot.h"

class StatCommand : public AdvisorCommand
{
public:
    StatCommand();
    void execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput) override;
};