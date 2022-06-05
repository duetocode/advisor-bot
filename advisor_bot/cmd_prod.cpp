#include "cmd_prod.h"
#include <string>
#include <sstream>
#include <set>

ProdCommand::ProdCommand() : AdvisorCommand("prod", CommandDescription{"list available products.", "prod"}) {}

void ProdCommand::execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput)
{
    // go over the records and using set to remove duplicates
    std::set<std::string> products;
    for (auto it = advisorBot.getOrderBook().begin(); it != advisorBot.getOrderBook().end(); it++)
    {
        for (auto entry : it->orders)
        {
            products.insert(entry.product);
        }
    }

    // output the result

    std::ostringstream oss;
    oss << "Available products: ";

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