#pragma once

#include "advisor_bot.h"

class HelpCommand : public AdvisorCommand
{
public:
    HelpCommand();
    void execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput) override;
};