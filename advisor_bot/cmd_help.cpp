#include "cmd_help.h"
#include <iomanip>

HelpCommand::HelpCommand() : AdvisorCommand(std::map<std::string, CommandDescription>{
                                 {"help", CommandDescription{
                                              "show this help message",
                                              "help"}},
                                 {"help cmd", CommandDescription{"show the usage of a specific command", "help <command>"}},
                             })
{
}

void HelpCommand::execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput)
{

    if (userInput.size() > 2)
    {
        advisorBot << "Invalid arguments. Usage: " << std::endl;
        for (const auto &pair : descriptions)
        {
            advisorBot << "\t" << pair.second.usage << std::endl;
        }
    }

    const auto &commands = advisorBot.getCommandsList();
    // if no arguments are given, show all commands
    if (userInput.size() == 1)
    {
        advisorBot << "Available commands:" << std::endl;

        // first, get the maximum length of the instructions
        int maxLength = 0;
        for (auto cmd : commands)
        {
            for (auto &desc : cmd->descriptions)
            {
                if (desc.first.length() > maxLength)
                {
                    maxLength = desc.first.length();
                }
            }
        }

        // then, print the instructions and descriptions
        for (auto cmd : commands)
        {
            for (auto &desc : cmd->descriptions)
            {
                advisorBot << "\t" << std::setw(maxLength) << std::left << desc.first << " - " << desc.second.description << std::endl;
            }
        }
    }
    else if (userInput.size() == 2)
    {
        // show the usage of a specific command
        CommandDescription *desc = nullptr;
        for (auto cmd : commands)
        {
            for (auto &desc : cmd->descriptions)
            {
                if (desc.first == userInput[1])
                {
                    advisorBot << desc.second.description << std::endl;
                    advisorBot << "Usage: " << desc.second.usage << std::endl;
                    return;
                }
            }
        }

        advisorBot << "No available information about " << userInput[1] << std::endl;
    }
}