// Implementation of the constructor 

#include "OrderBookEntry.h"

// Include it with OrderBook::OrderBook
OrderBookEntry::OrderBookEntry( double _price,  
                                double _amunt, 
                                std::string _timestamp, 
                                std::string _product,
                                OrderBookType _orderType, 
                                std::string _username)
// member initialisation list 
: price(_price),
  amount(_amunt),
  timestamp(_timestamp),
  product(_product),
  orderType(_orderType),
  username(_username)
{
   
}      

OrderBookType OrderBookEntry::stringToOrderBookType(std::string s)
{
  if (s == "ask")
  {
    return OrderBookType::ask;
  }
  if (s == "bid")
  {
    return OrderBookType::bid;
  }
  return OrderBookType::unknown;
}
