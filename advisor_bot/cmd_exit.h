#pragma once

#include "advisor_bot.h"

/* stop the program */
class ExitCommand : public AdvisorCommand
{
public:
    ExitCommand();
    virtual void execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput) override;
};