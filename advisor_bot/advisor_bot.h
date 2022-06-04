#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "order_book.h"

class AdvisorBot;

struct CommandDescription
{
    std::string description;
    std::string usage;
};

/* Base class for all commands. */
class AdvisorCommand
{
public:
    /** description of the command. Can have multiple lines. Will be displayed by the help command*/
    const std::map<std::string, CommandDescription> descriptions;

    /** @brief default contructor, initializes the object with name and descriptions
     * @param descriptions the description of the command, can have multiple lines
     */
    AdvisorCommand(std::map<std::string, CommandDescription> descriptions);
    /** convenience constructor for commands with only one usage information */
    AdvisorCommand(std::string instruction, CommandDescription);

    /**
     * @brief run the command
     *
     * @param advisorBot the bot object, acting as a context for the command
     * @param userInput the user input, split by spaces
     */
    virtual void execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput) = 0;
};

/* Class responsible for interacting with the user, which includes handling user input and display feedback. */
class AdvisorBot
{
public:
    /* default constructor */
    AdvisorBot();
    /**
     * @brief Main loop of the advisor bot.
     */
    void run();
    /**
     * @brief stop the main loop.
     */
    void stop();

    const std::vector<AdvisorCommand *> &getCommandsList() const;
    OrderBook &getOrderBook();

    /* output message through the bot */
    template <typename T>
    std::ostream &operator<<(T &&t)
    {
        return std::cout << "advisorbot> " << t;
    }

private:
    /* command objects */
    std::vector<AdvisorCommand *> commandList;
    OrderBook orderBook;
    /** flag indicats the state of the application */
    bool running;
    /* lookup table for command matching */
    std::map<std::string, AdvisorCommand *> commandTable;
};