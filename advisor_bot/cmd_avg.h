#pragma once

#include "advisor_bot.h"

/** compute average price by specified time steps, product, and order type. */
class AvgCommand : public AdvisorCommand
{
public:
    AvgCommand();
    void execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput) override;
};