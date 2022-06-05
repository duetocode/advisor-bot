#pragma once

#include "advisor_bot.h"

class PredictCommand : public AdvisorCommand
{
public:
    PredictCommand();
    void execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput) override;
};