#pragma once

#include "advisor_bot.h"

class TimeCommand : public AdvisorCommand
{
public:
    TimeCommand();
    void execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput) override;
};