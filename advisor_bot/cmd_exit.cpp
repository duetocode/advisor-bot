#include "cmd_exit.h"

ExitCommand::ExitCommand() : AdvisorCommand(
                                 "exit", CommandDescription{
                                             "Exit the program",
                                             "exit"})
{
}

void ExitCommand::execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput)
{
    advisorBot.stop();
    advisorBot << "Bye!" << std::endl;
}