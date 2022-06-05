# Introduction

AdvisorBot is a command line program created for the OOP midterm assignment which can analyse the trading data in the csv file. It can do some basic statistical information to help cryptocurrency traders get some insights about the current market. It also implemented a prediction function based on Exponential Smoothing.

The development process is a great opportunity to learn and practise C++ language and OOP design. 

# Build

The project is managed by CMake. 

## Required Pakcages

* C++ compiler supports C++17
* CMake
* Ninja or GNU make
* Google Test

## Build

This example uses ninja:

1. Create a build directory and enter
    ```bash
    > mkdir build
    > cd build
    ```
2. Configuration

    ```bash
    > cmake -DCMAKE_BUILD_TYPE=Release -GNinja ..
    ```
3. Build
    ```bash
    > cmake --build .
    ```
4. The program should be in ```build/advisorbot```

