// Only include this file once
#pragma once

#include <vector>
#include <string>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

// Contains all functions TradeUp can do
class TradeUpMain
{
  public:
    TradeUpMain();
    //prototype functions
    /**Call this to start the application */
    void init();



  private:
    void errorHandeling();
    void printHelp();
    void showMarketStats();
    void startSelling();
    void startBuying();
    void printWallet();
    void endProgram();
    void printMenu();
    int getUserInput();
    void processInput(int userInput);
    void continueTime();

    //All advisorBot methods
    /** Use the AdviceBot, enters the main while loop and handels the user input*/
    void useAdviceBot();
    /** Logs help for all commnads to the console*/
    void printBotHelp();
    /** Logs all availabele products to the console*/
    void printProds();
    /** One function to calculate the min or max and log it to the console*/
    void outputMinMax(std::vector<std::string>& processedInput);
    /** Calculates the average for a product and ask or bid type, and logs the result to the console*/
    void outputAvg(std::vector<std::string>& processedInput);
    /** Uses three data points: current, one and two before; to calculate a weighted average to predict the next value*/
    void outputPredict(std::vector<std::string>& processedInput);

    std::string currentTime;

    OrderBook orderBook{"2020-03-17.csv"};

    Wallet wallet;

    std::vector<std::string> allProducts;
    


};