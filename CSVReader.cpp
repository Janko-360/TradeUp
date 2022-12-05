#include "CSVReader.h"
#include <iostream> 
#include <fstream>

CSVReader::CSVReader()
{

}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFileName )
{
  std::vector<OrderBookEntry> entries;

  std::ifstream csvFile{csvFileName};
  std::string line;

  if (csvFile.is_open())
  {
    while(std::getline(csvFile, line))
    {
      try {
        OrderBookEntry obj = stringsToOBE(tokenise(line, ','));
        entries.push_back(obj);
      } catch (const std::exception& e)
      {
          // possible error messages 
          // std::cout << "CSVReader::readCSV bad data!" << std::endl ;
      }
    }
  } 
  // Possible error messages
  // std::cout << "CSVReader::readCSV read " << entries.size() << " entries." << std::endl ;
  return entries;
}




std::vector<std::string> CSVReader::tokenise(std::string csvLine, char seperator)
{
  std::vector<std::string> tokens; 
  signed int start, end; 
  std::string token;
  start = csvLine.find_first_not_of(seperator, 0); 

  do{
    end = csvLine.find_first_of(seperator, start);

    if (start == csvLine.length() || start == end) break; 
    if (end >= 0) token = csvLine.substr(start, end - start);
    else token = csvLine.substr(start, csvLine.length() - start);

    tokens.push_back(token);
    start = end + 1;
  } while (end > 0);

  return tokens; 
}

OrderBookEntry CSVReader::stringsToOBE(std::string priceString, 
                                    std::string amountString, 
                                    std::string timestamp,
                                    std::string product,
                                    OrderBookType  OrderType)
{
  double price, amount;

  try
  {
    price = std::stod(priceString);
    amount = std::stod(amountString);
  } catch (const std::exception& e) 
  {
    std::cout << "CSVReader::stringsToOBE -> Bad float: " << priceString << std::endl;
    std::cout << "CSVReader::stringsToOBE -> Bad float: " << amountString << std::endl;
    throw;
  }

  OrderBookEntry obj{price,
                      amount,
                      timestamp,
                      product,
                      OrderType };
  return obj;

}

OrderBookEntry CSVReader::stringsToOBE(std::vector <std::string> tokens)
{
  double price, amount;

  if (tokens.size() != 5)
  {
    // std::cout << "Err: We don't have all values in the row" << std::endl;
    throw std::exception{};
  }

  try
  {
    price = std::stod(tokens[3]);
    amount = std::stod(tokens[4]);
    // std::cout << "ID: " << tokens[0] << std::endl;  

  } catch(const std::exception& e) 
  {
    // std::cout << "CSVReader::stringsToOBE encountered an error converting the data value" << std::endl;
    throw;
  }
      
  OrderBookEntry obj{price,
                      amount,
                      tokens[0],
                      tokens[1],
                      OrderBookEntry::stringToOrderBookType(tokens[2])};
  
  return obj; 
}