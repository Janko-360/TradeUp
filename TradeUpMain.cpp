#include <iostream>
#include <vector>

#include "TradeUpMain.h"
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "math.h" // for the power function in predict


TradeUpMain::TradeUpMain()
{
}

void TradeUpMain::init()
{
    int input = 0; 
    currentTime = orderBook.getEarliestTime();
    wallet.insertCurrency("BTC", 10);
    allProducts = orderBook.getKnownProducts();
    while(input!=8)
    {
        printMenu();
        input = getUserInput();
        processInput(input);
    }
}

void TradeUpMain::errorHandeling()
{
    std::cout << "Invalid input" << std::endl;
    std::cout << "Please pick options 1-8" << std::endl;
}

void TradeUpMain::printHelp()
{
    std::cout << "HELP" << std::endl;
    std::cout << "Study the market. Buy smart and sell smarter to make money. Good luck." << std::endl;
}

void TradeUpMain::useAdviceBot(){
    // Introduction text, gets shown once
    std::cout << "Welcome to the AdvisotBot \n------------------------------------------------------" << std::endl;
    std::cout << "Please enter a command \n    help for a list of all commands\n    exit or e to exit to the main menu" << std::endl; 
    std::string botInput = " ";

    // a loop to stay in the advisorBot to properly use the advisorBot. 
    while(botInput!="exit" && botInput!="e")
    {
        std::cout << " " << std::endl;
         std::cout << "---------------------------------------------- " << std::endl;
        std::cout << "Please enter a command   (exit or e to exit)" << std::endl; 
        std::getline(std::cin, botInput);
        std::cout << " " << std::endl;         

        // first simple commands and the possible variations
        // all help commands and variations  
        // only realy long or calculated prints use a function, the others stay in-line
        if (botInput == "help" || botInput == "help ") 
        {
            printBotHelp();
        } 
        else if (botInput == "exit" || botInput == "e" || botInput == "exit " || botInput == "e ")
        {
            break;
        }
        else if (botInput == "help time" || botInput == "help time ")
        {
            std::cout << "The time command will show the current time." << std::endl; 
        }
        else if (botInput == "help step" || botInput == "help step ")
        {
            std::cout << "The step command will step to the next time slice." << std::endl; 
        }
        else if (botInput == "help prod" || botInput == "help prod ")
        {
            std::cout << "The prod command will list all items on the market for the current time slice" << std::endl; 
        }
        else if (botInput == "help avg" || botInput == "help avg ")
        {
            std::cout << "Calculates the average for a product over a past number of time slices" << std::endl;
            std::cout << "The Input formatting should be: avg <product> <bid/ask> <number of time slices>" << std::endl;
        }
        else if (botInput == "help max" || botInput == "help max " )
        {
            std::cout << "Get the maximum price for a product in the current time slice" << std::endl;
            std::cout << "The Input formatting should be: max <product> <bid/ask>" << std::endl;
        }
        else if (botInput == "help min" || botInput == "help min ")
        {
            std::cout << "Get the minimum price for a product in the current time slice" << std::endl;
            std::cout << "The Input formatting should be: min <product> <bid/ask>" << std::endl;
        }
        else if (botInput == "help predict" || botInput == "help predict ")
        {
            std::cout << "Get a prediction for the next step" << std::endl;
            std::cout << "The Input formatting should be: predict <max/min> <product> <bid/ask>" << std::endl;
        }
        else if (botInput == " ")
        {
            std::cout << "Please enter a valid command and parameters" << std::endl; 
        }
        else if (botInput == "time" || botInput == "time ")
        {
            std::cout << "Current time: " << currentTime << std::endl;
        }
        else if (botInput == "step" || botInput == "step ")
        {
            TradeUpMain::continueTime();                                    
            std::cout << "Time is: " << currentTime << std::endl;
        }
        else if (botInput == "prod" || botInput == "prod ")
        {
            TradeUpMain::printProds();
        }
        
        // first look for a space, then check to see what the next parameters  might be
        else if (botInput.find(" ") != std::string::npos) 
        {
            std::vector<std::string> processedInput;
            // Tokenisation for efficient use of parameters  
            try {
                processedInput = CSVReader::tokenise(botInput, ' ');     
            } catch (const std::exception& e)
            {
                std::cout << "TradeUpMain::useAdviceBot -> Can't work with the input" << std::endl;
            }

            if (processedInput[0] == "min")
            {
                outputMinMax(processedInput);
            }
            else if (processedInput[0] == "max")
            {
                outputMinMax(processedInput);
            }
            else if (processedInput[0] == "avg")
            {
               outputAvg(processedInput);
            }
            else if (processedInput[0] == "predict")
            {
                outputPredict(processedInput);
            }
            else 
            {
                std::cout << "Please enter a valid command and parameters" << std::endl; 
            }
        // end of command with parameters check
        }
        else 
        {
            std::cout << "Please enter a valid command and/or parameters" << std::endl; 
        }
    // end of advisorBot while loop
    }    
}

void TradeUpMain::printBotHelp()
{
    std::cout << "The AdvisorBot is here to help you analyse  the market" << std::endl;  
    std::cout << "Available commands are: \n    help\n    help <cmd> (for help on a specific command)\n    prod\n    time\n    step\n    exit or 'e'" << std::endl;
    std::cout << "    min <product> <ask/bid>\n    max <product> <ask/bid>" << std::endl;
    std::cout << "    avg <product> <ask/bid> <number of time steps>\n    predict <max/min> <product> <ask/bid>" << std::endl;
}

void TradeUpMain::printProds()
{
    std::cout << "Available products are: " << std::endl; 
    for (std::string const& p : allProducts)
    {
        std::cout << "    " << p << std::endl;
    }
}

void TradeUpMain::outputMinMax(std::vector<std::string>& processedInput)
{
    // Format max/min <product> <ask/bid>
    try {
        // First some input validation
        std::string product = processedInput[1];
        std::string operation = processedInput[2];

        // A varaiable to check if we could find the given product. 
        bool managedOutput = false;

        if (operation != "ask" && operation != "bid")
        {
            throw std::string("input does not contain a ask or bid parameter");
        }

        // Actual max or min calculation 
        for (std::string const& p : allProducts)
        {
            if (p == product)
            {
                std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookEntry::stringToOrderBookType(operation), 
                                                                        p, currentTime);
                if (processedInput[0] == "max")
                {
                    std::cout << "Max " << operation << " for " << p << " is: "  << OrderBook::getHighPrice(entries) << std::endl; 
                }
                else
                {
                    std::cout << "Min " << operation << " for " << p << " is: "  << OrderBook::getLowPrice(entries) << std::endl; 
                }
                
                managedOutput = true;
            }
            else {
                continue; 
            }
        }

        if (!managedOutput) {throw std::string("You entered a wrong product or it is not found");}

    } 
    catch (std::string e)
    {
        std::cout << "Error: " << e << std::endl; 
    } 
    catch (...)
    {
        std::cout << "Please enter a valid command and parameters \nThe Input formatting should be: <min or max> <product> <bid or ask>" << std::endl; 
    }
}

void TradeUpMain::outputAvg(std::vector<std::string>& processedInput)
{
    // Format avg <product> <ask/bid> <number of time slices> 
    try {
        // First input validation
        int iter = std::stoi(processedInput[3]);
        if (iter <= 0)
        {
            throw std::string("input must contain a positive time period number");
        }

        std::string operation = processedInput[2];
        if (operation != "ask" && operation != "bid")
        {
            throw std::string("input does not contain a ask or bid parameter");
        }

        // Check if the product exists 
        bool prodCheckingBool = false;
        for (std::string const& p : allProducts)
        { 
            if (processedInput[1] == p)
            {
                prodCheckingBool = true;
            } 
            else
            {
                continue;
            }
        }
        if(!prodCheckingBool){throw std::string("You entered a wrong product or it is not found");}

        // calculation variables 
        std::string useTime = currentTime;
        double total = 0; 
        int nCount = 0; 

        bool tooLongCount = false;


        // Actual calculation of the average 
            // going from curent time to the first time slice (back in time)
        for (int i = 1; i <= iter; i++) {
            if ( useTime == "" || tooLongCount != 0)  // second condition prevents calculation with data looping back from the oldest time
            {
                tooLongCount = true ;
            }
            else 
            {
                std::vector<OrderBookEntry> entries = orderBook.getOrders( OrderBookEntry::stringToOrderBookType(operation), 
                                                                    processedInput[1], 
                                                                    useTime);

                total += OrderBook::getAvgPrice(entries);
                nCount ++;
            } 
            useTime = orderBook.getPreviousTime(useTime);
        }
        if (tooLongCount) {std::cout << "The calculated average can only include the last " << nCount << " time periods." << std::endl;}

        std::cout << "The average " << processedInput[1] << " " <<  operation << " price is: " << total/nCount << std::endl;
    }
    catch (std::string e)
    {
        std::cout << "Error: " << e << std::endl; 
    } 
    catch (...)
    {
        std::cout << "Invalid input for the average calculation \nThe Input formatting should be: avg <product> <bid or ask> <number of time slices>" << std::endl;
    }
}

void TradeUpMain::outputPredict(std::vector<std::string>& processedInput)
{
    // Format predict <max/min> <product> <ask/bid>
    try {
        // First input validation
        // A indirect check if we have all parameters  (cant assing processedInput[3] if it is not there) 
        std::string min_max = processedInput[1];
        std::string product = processedInput[2];
        std::string operation = processedInput[3];

        if (min_max != "min" && min_max != "max" )
        {
            throw std::string("input does not contain a min or max parameter");
        }

        if (operation != "ask" && operation != "bid")
        {
            throw std::string("input does not contain a ask or bid parameter");
        }

        // Check if the product exists 
        bool prodCheckingBool = false;
        for (std::string const& p : allProducts)
        { 
            if (processedInput[2] == p)
            {
                prodCheckingBool = true;
            } 
            else
            {
                continue;
            }
        }
        if(!prodCheckingBool){throw std::string("You entered a wrong product or it is not found");}

        // Calculation varaibles
        std::string useTime = currentTime;
        double weightedTotal = 0; 
        float nCount = 0; 

        // Actual prediction algorithm
        for (int i = 0; i <= 2; i++) {
            // this is to prevent looping back to the oldest value, and ensuring only valid data
            if (useTime == "") {break;}

            // weight used for the current data point
            double currentWeiht = 1/pow(2, i);

            if ( min_max == "max")
            {
                std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookEntry::stringToOrderBookType(operation), 
                                                                        product, useTime);
                weightedTotal += OrderBook::getHighPrice(entries) * currentWeiht;
            }
            else if (min_max == "min")
            {
                std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookEntry::stringToOrderBookType(operation), 
                                                                        product, useTime);
                weightedTotal += OrderBook::getLowPrice(entries) * currentWeiht;
            }
            else {
                continue; 
            }

            nCount += currentWeiht;
            useTime = orderBook.getPreviousTime(useTime);
        }
        // nCount must equal 1.75, if not then we don't use three time slices to calculate the next value and the prediction is inaccurate
        if (nCount != 1.75)
        {
            std::cout << "There are too few time steps to calculate a accurate prediction value, try progressing several time steps" << std::endl;
        }
        else
        {
            std::cout << "The " << min_max << " " << product << " " << operation << " price should be: " << weightedTotal/nCount << std::endl; 
        }
    } 
    catch (std::string e)
    {
        std::cout << "Error: " << e << std::endl; 
    } 
    catch (...)
    {
        std::cout << "Please enter a valid command and parameters \nThe Input formatting should be: predict <max/min> <product> <bid/ask>" << std::endl; 
    }
}

//Main menu functions
void TradeUpMain::showMarketStats()
{
    std::cout << "MARKET STATS:" << std::endl;

    for (std::string const& p : allProducts)
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, 
                                                                p, currentTime);
        std::cout << "    Asks seen: " << entries.size() << std::endl;
        std::cout << "    Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "    Min ask: " << OrderBook::getLowPrice(entries) << std::endl;
        std::cout << "    Average price: " << OrderBook::getAvgPrice(entries) << std::endl;
    }
}

void TradeUpMain::startSelling()  // Ask, ask this mush for a product 
{
    std::cout << "START AN OFFER" << std::endl;
    std::cout << "Enter the details: (product,price,amount) No spaces! " << std::endl;
    std::string input; 
    // get the line from console in AND store it imidiatley to input. 
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "TradeUpMain::startSelling -> Please enter correct formated info" << std::endl; 
    } else {
        try {
           OrderBookEntry obj = CSVReader::stringsToOBE(
            tokens[1],
            tokens[2],
            currentTime,
            tokens[0],
            OrderBookType::ask); 
            obj.username = "simuser";

            if (wallet.canFulfillOrder(obj))
            {
                std::cout << "Wallet loos good for the purchase" << std::endl;
                orderBook.insertOrder(obj);
            } else {
                std::cout << "Your ballence is not sufficient" << std::endl;
            }

        } catch (const std::exception& e)
        {
            std::cout << "TradeUpMain::startSelling -> Can't create a order object." << std::endl;
        }
    }

    std::cout << "You typed: " << input << std::endl;
}

void TradeUpMain::startBuying()  //Bid, offer to buy this much for a porduct 
{
    std::cout << "START A PURCHASE" << std::endl;
    std::cout << "Enter the details for a bid: (product,price,amount) No spaces!" << std::endl;

    std::string input; 
    // get the line from console in AND store it imidiatley to input. 
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "TradeUpMain::startBuying -> Please enter correct formated info" << std::endl; 
    } else {
        try {
           OrderBookEntry obj = CSVReader::stringsToOBE(
            tokens[1],
            tokens[2],
            currentTime,
            tokens[0],
            OrderBookType::bid); 
            obj.username = "simuser";

            if (wallet.canFulfillOrder(obj))
            {
                std::cout << "Wallet loos good for the purchase" << std::endl;
                orderBook.insertOrder(obj);
            } else {
                std::cout << "Wallet has infufficient funds" << std::endl;
            }

        } catch (const std::exception& e)
        {
            std::cout << "TradeUpMain::startBuying -> Can't create a order object." << std::endl;
        }
    }
}

void TradeUpMain::printWallet()
{
    std::cout << "YOUR WALLET:" << std::endl;
    std::cout << wallet.toString() << std::endl;
}

void TradeUpMain::continueTime()
{
    std::cout << "Next time frame" << std::endl;
    std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids("ETH/BTC", currentTime);
    std::cout << "Sales: " << sales.size() << std::endl;
    for (OrderBookEntry& sale : sales)
    {
        std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
        if (sale.username == "simuser")
        {
            wallet.processSale(sale);
        }
    }
    currentTime = orderBook.getNextTime(currentTime);
}

void TradeUpMain::endProgram()
{
    std::cout << "Good Bye" << std::endl;
}        

void TradeUpMain::printMenu()
{
    std::cout << "" << std::endl;
    std::cout << "Current time is: " << currentTime << std::endl;
    // 1 print help
    std::cout << "1: Help" << std::endl;
    // 2 use the advisorbot
    std::cout << "2: AdvisorBot" << std::endl;
    // 3 View market stats
    std::cout << "3: View Market" << std::endl;
    // 4 make a offer (sell)
    std::cout << "4: Make a offer" << std::endl;
    // 5 make a bid (buy)
    std::cout << "5: Place a bid" << std::endl;
    // 6 view wallet
    std::cout << "6: View wallet" << std::endl;
    // 7 time step 
    std::cout << "7: Next turn" << std::endl;
    // 8 end program
    std::cout << "8: End program" << std::endl;
    std::cout << "---------------------" << std::endl;
    std::cout << "Select option (1-8)" << std::endl;
}

int TradeUpMain::getUserInput()
{
    int userInput = 0;
    std::string line; 
    std::getline(std::cin, line);
    try {
            userInput = std::stoi(line);
    } catch (const std::exception& e)
    {
        // just continue, the default of 0 won't be changed and the program continues 
    }
    std::cout << " " << std::endl;
    return userInput;
}

void TradeUpMain::processInput(int userInput)
{
    if (userInput == 0) // bad input
    {
        errorHandeling();
    }
    if (userInput == 1) // help
    {
        printHelp();
    }
    if (userInput == 2) // advisorbot
    {
        useAdviceBot();
    }
    if (userInput == 3) //show market
    {
        showMarketStats();
    }
    if (userInput == 4) // sell
    {
        startSelling();
    }
    if (userInput == 5) // buy
    {
        startBuying();
    }
    if (userInput == 6) // view wallet
    {
        printWallet();
    }
    if (userInput == 7) // next turn
    {
        continueTime();
    }
    if (userInput == 8) // end
    {
        endProgram();
    }
    if (userInput >= 9) // catch everything else
    {
        errorHandeling();
    }
}