#include "advisor_bot.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <chrono>

#include "order_book.h"

#include "cmd_help.h"
#include "cmd_exit.h"
#include "cmd_prod.h"
#include "cmd_min.h"
#include "cmd_max.h"
#include "cmd_step.h"
#include "cmd_avg.h"
#include "cmd_time.h"
#include "cmd_predict.h"
#include "cmd_stat.h"

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
    // intialize all command objects
    commandList = {
        new HelpCommand{},
        new ProdCommand{},
        new MinCommand{},
        new MaxCommand{},
        new AvgCommand{},
        new PredictCommand{},
        new TimeCommand{},
        new StepCommand{},
        new StatCommand{},
        new ExitCommand{},
    };

    // register command objects to the look up table
    for (auto &cmd : commandList)
    {
        for (auto &desc : cmd->descriptions)
        {
            commandTable[desc.first] = cmd;
        }
    }

    // intialize the order book
    (*this) << "Welcome! Advisor Bot at your service." << std::endl;
    (*this) << "Loading order book..." << std::endl;
    auto tBegin = std::chrono::steady_clock::now();
    orderBook = std::move(OrderBook{"data/20200601.csv"});
    auto tEnd = std::chrono::steady_clock::now();
    (*this) << "Order book loaded in " << std::chrono::duration_cast<std::chrono::seconds>(tEnd - tBegin).count() << "s" << std::endl;

    // intialize the step iterator and leave it at the first step
    it = orderBook.begin();
}

void AdvisorBot::stop()
{
    running = false;
}

void AdvisorBot::run()
{
    running = true;
    it = orderBook.begin();

    // welcome message
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

        // execute command
        AdvisorCommand *command = commandTable.at(userInput[0]);
        command->execute(*this, userInput);
    }
}