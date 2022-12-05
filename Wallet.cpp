#include "Wallet.h"
#include "CSVReader.h"

#include <iostream> 
#include <string> 
#include <vector> 

Wallet::Wallet()
{

}

void Wallet::insertCurrency(std::string type, double amount)
{
    double balance; 
    if ( amount < 0)
    {
        std::cout << "Err: Wallet::insertCurrency -> negative amount" << std::endl;
        throw std::exception{};
    }

    if (currencies.count(type) == 0) // product type is not in the wallet yet
    {
        balance = 0;
    } else {  // the product is in the wallet alteady 
        balance = currencies[type]; 
    }
    balance += amount;
    currencies[type] = balance;
}

bool Wallet::removeCurrency(std::string type, double amount)
{
    if ( amount < 0)
    {
        return false;
    }

    if (currencies.count(type) == 0) // product type is not in the wallet yet
    {
        return false;
    } else {  // the product is in the wallet - do we have enough
        if (containsCurrency(type, amount))
        {
            currencies[type] -= amount; 
            return true;
        } else {
            return false;
        }
    }
}


bool Wallet::containsCurrency(std::string type, double amount)
{
    if (currencies.count(type) == 0)
        return false;
    else
        return currencies[type] >= amount;
}

bool Wallet::canFulfillOrder(OrderBookEntry order)
{
    std::vector<std::string> currs = CSVReader::tokenise(order.product, '/');
    // ask 
    if (order.orderType == OrderBookType::ask)
    {
        double amount = order.amount; 
        std::string currency = currs[0];
        std::cout << "Wallet::canFulfillOrder " << currency << amount << std::endl;
        return containsCurrency(currency, amount);
    }
    //bid
    if (order.orderType == OrderBookType::bid)
    {
        double amount = order.amount * order.price; 
        std::string currency = currs[1];
        std::cout << "Wallet::canFulfillOrder " << currency << amount << std::endl;
        return containsCurrency(currency, amount);
    }
    return false;  //if we can't fulfill the order or other error 
}

void Wallet::processSale(OrderBookEntry& sale)
{
    std::vector<std::string> currs = CSVReader::tokenise(sale.product, '/');
    // ask 
    if (sale.orderType == OrderBookType::asksale)
    {
        double outGoingAmount = sale.amount; 
        std::string outGoingCurrency = currs[0];
        double incommingAmount = sale.amount * sale.price;
        std::string incommingCurrency = currs[1];

        currencies[incommingCurrency] += incommingAmount;
        currencies[outGoingCurrency] -= outGoingAmount;
    }
    //bid
    if (sale.orderType == OrderBookType::bidsale)
    {
        double incommingAmount = sale.amount; 
        std::string incommingCurrency = currs[0];
        double outGoingAmount = sale.amount * sale.price;
        std::string outGoingCurrency = currs[1];

        currencies[incommingCurrency] += incommingAmount;
        currencies[outGoingCurrency] -= outGoingAmount;
    }
}

std::string Wallet::toString()
{
    std::string s; 
    for (std::pair<std::string, double> pair : currencies)
    {
        std::string currency = pair.first; 
        double amount = pair.second; 
        s += currency + ": " + std::to_string(amount) + "\n"; 
    }
    return s;
}