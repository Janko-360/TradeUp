#pragma once

#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string> 
#include <vector>

class OrderBook
{
  public: 
    /** construct, reading a csv data file */
    OrderBook(std::string filename);
    /** return vector of all known product combinations in the dataset*/
    std::vector<std::string> getKnownProducts();
    /** return vector of Orders according to the sent filters*/
    std::vector<OrderBookEntry> getOrders(OrderBookType type, 
                                        std::string product,
                                        std::string timestamp);

    // Data functions
    /** calculates the average price for the current time stamp */ 
    static double getAvgPrice(std::vector<OrderBookEntry>& orders);

    /** returns the highest price value for a time period */
    static double getHighPrice(std::vector<OrderBookEntry>& orders);
    /** returns the lowest price value for a time period  */
    static double getLowPrice(std::vector<OrderBookEntry>& orders);

    /** Get the first timestamp, as a string */
    std::string getEarliestTime();
    /** Get the next timestamp after the sent time, as a string 
     * If there is no next timestamp, it wraps around to the start*/
    std::string getNextTime(std::string timestamp);
    /** Gets the preceding time stamp to the given time stamp*/ 
    std::string getPreviousTime(std::string timestamp);

    void insertOrder(OrderBookEntry& order);

    /**The Mathing engine. Takes a product combination and a timestamp, returns a vector of sales and changes the database to not repeat the sale */
    std::vector<OrderBookEntry> matchAsksToBids(std::string products, std::string timestamp);

  private: 
    std::vector<OrderBookEntry> orders; 
};