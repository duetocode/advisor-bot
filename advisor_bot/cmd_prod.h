#pragma once

#include "advisor_bot.h"

class ProdCommand : public AdvisorCommand
{
public:
    ProdCommand();
    void execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput) override;
};