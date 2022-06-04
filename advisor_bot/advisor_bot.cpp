#include "advisor_bot.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "order_book.h"

#include "cmd_help.h"
#include "cmd_exit.h"
#include "cmd_prod.h"
#include "cmd_min.h"
#include "cmd_max.h"
#include "cmd_step.h"
#include "cmd_avg.h"

AdvisorCommand::AdvisorCommand(std::map<std::string, CommandDescription> descriptions) : descriptions(descriptions) {}
AdvisorCommand::AdvisorCommand(std::string instruction, CommandDescription description)
    : AdvisorCommand(std::map<std::string, CommandDescription>{{instruction, description}}) {}

static std::vector<std::string> getUserInput()
{
    // buffer for user input
    std::string line{};
    // tokenized user input
    std::vector<std::string> result;

    // read one line from stdin
    std::cout << "user> ";
    std::getline(std::cin, line);

    // tokenize user input by whitespace
    std::istringstream iss{line};
    std::string token;
    while (std::getline(iss, token, ' '))
    {
        result.push_back(token);
    }

    // filter out whitespace tokens
    for (auto it = result.begin(); it != result.end();)
    {
        if (*it == "")
            it = result.erase(it);
        else
            ++it;
    }

    return result;
}

const std::vector<AdvisorCommand *> &AdvisorBot::getCommandsList() const
{
    return commandList;
}

OrderBook &AdvisorBot::getOrderBook()
{
    return orderBook;
}

AdvisorBot::AdvisorBot()
{
    commandList = {
        new HelpCommand{},
        new AvgCommand{},
        new ProdCommand{},
        new MinCommand{},
        new MaxCommand{},
        new StepCommand{},
        new ExitCommand{},
    };

    for (auto &cmd : commandList)
    {
        for (auto &desc : cmd->descriptions)
        {
            commandTable[desc.first] = cmd;
        }
    }

    // orderbook = std::move(OrderBook{"data/20200601.csv"});
    orderBook = std::move(OrderBook{"test/test_data.csv"});
}

void AdvisorBot::stop()
{
    running = false;
}

void AdvisorBot::run()
{
    running = true;
    // welcome message
    *this << "Welcome to the Advisor Bot!" << std::endl;
    *this << "Please enter a command, or help for a list of commands" << std::endl;

    while (running)
    {
        std::vector<std::string> userInput = getUserInput();

        // ignore blank inputs
        if (userInput.empty())
            continue;

        // unsupported command
        if (commandTable.count(userInput[0]) == 0)
        {
            *this << "Unknown command: " << userInput[0] << std::endl;
            continue;
        }

        AdvisorCommand *command = commandTable.at(userInput[0]);
        command->execute(*this, userInput);
    }
}