#include "cmd_predict.h"
#include <string>
#include <sstream>
#include "utils.h"

PredictCommand::PredictCommand() : AdvisorCommand("predict", CommandDescription{"Predict the price 1 step ahead.", "predict <product>"}) {}

void PredictCommand::execute(AdvisorBot &advisorBot, std::vector<std::string> &userInput)
{
    // check the argument
    if (userInput.size() != 2)
    {
        advisorBot << "Invalid arguments. Usage: " << descriptions.begin()->second.usage << std::endl;
        return;
    }

    // Using Exponential Smoothing https://en.wikipedia.org/wiki/Exponential_smoothing
    // Predict based on the average of the min ask and max bid price of the last 2 time steps

    // First, match and calcuate the average price in the last 2 time steps
    const auto &steps = advisorBot.getOrderBook().getPreviousSteps(10);
    std::vector<double> averageMatchedPrices;
    for (auto it = steps.rbegin(); it != steps.rend() && averageMatchedPrices.size() <= 2; ++it)
    {
        auto matched = (*it)->matchAsksToBids(userInput[1]);

        // if no orders found, just skip this step
        if (matched.empty())
            continue;

        // calculate the average price
        double sum = 0, amount = 0;
        for (auto &order : matched)
        {
            sum += order.price * order.amount;
            amount += order.amount;
        }
        double average = sum / amount;
        averageMatchedPrices.push_back(average);
    }

    // the first prediction is equal to the first value in the series since we do not have enough data to predict
    const static double alpha = 0.2;
    if (averageMatchedPrices.empty())
    {
        advisorBot << "No historical data in the past 10 steps to predict." << std::endl;
    }
    else if (averageMatchedPrices.size() == 1)
    {
        // we only have one historical record, so just return the average
        // s0 = x0
        advisorBot << "Prediction: " << averageMatchedPrices[0] << std::endl;
    }
    else
    {
        // s_t = alpha * x_t + (1 - alpha) * x_t-1
        double prediction = alpha * averageMatchedPrices[0] + (1 - alpha) * averageMatchedPrices[1];
        advisorBot << "Prediction: " << prediction << std::endl;
    }
}