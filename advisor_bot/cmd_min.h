#pragma once

#include "advisor_bot.h"

class MinCommand : public AdvisorCommand
{
public:
    MinCommand();
    void execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput) override;
};