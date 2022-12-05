#pragma once 

#include "OrderBookEntry.h"
#include <vector>
#include <string>

class CSVReader
{
  public: 
    CSVReader();

    static std::vector<OrderBookEntry> readCSV(std::string csvFile);

    /** Tokenise takes a csv string and a seperator to break up the string to usable objects */
    static std::vector<std::string> tokenise(std::string csvLine, char seperator);
    /** PUBLIC function, takes all OrdBokEnrty parameters, converts to doubles and RETURNS a OrdBokEnrty*/
    static OrderBookEntry stringsToOBE(std::string price, 
                                        std::string amount, 
                                        std::string timestamp,
                                        std::string product,
                                        OrderBookType  OrderBookType);

  private: 
    /* PRIVATE function, takes a vector of strings and retrns a OrderBookEntry Object*/
    static OrderBookEntry stringsToOBE(std::vector <std::string> strings);


};