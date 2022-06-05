#pragma once

#include "advisor_bot.h"

/** display help messages */
class HelpCommand : public AdvisorCommand
{
public:
    HelpCommand();
    void execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput) override;
};