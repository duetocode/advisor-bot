#include "cmd_prod.h"
#include <string>
#include <sstream>

ProdCommand::ProdCommand() : AdvisorCommand("prod", CommandDescription{"list available products.", "prod"}) {}

void ProdCommand::execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput)
{
    std::ostringstream oss;

    oss << "Available products: ";

    auto products = advisorBot.getOrderBook().getAvailableProducts();

    // join the products as a comma separated string
    for (auto it = products.begin(); it != products.end(); ++it)
    {
        if (it != products.begin())
        {
            oss << ", ";
        }
        oss << *it;
    }

    // output
    advisorBot << oss.str() << std::endl;
}